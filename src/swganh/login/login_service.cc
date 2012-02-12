/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "swganh/login/login_service.h"

#include <glog/logging.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "anh/app/kernel_interface.h"

#include "anh/database/database_manager.h"

#include "anh/event_dispatcher.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/server.h"

#include "anh/service/service_directory_interface.h"
#include "anh/service/service_manager.h"
#include "anh/plugin/plugin_manager.h"

#include "swganh/base/swg_message_handler.h"

#include "swganh/login/messages/enumerate_character_id.h"
#include "swganh/login/messages/error_message.h"
#include "swganh/login/messages/login_client_token.h"
#include "swganh/login/messages/login_cluster_status.h"
#include "swganh/login/messages/login_enum_cluster.h"

#include "swganh/login/authentication_manager.h"
#include "swganh/login/login_client.h"
#include "swganh/login/encoders/sha512_encoder.h"
#include "swganh/login/providers/mysql_account_provider.h"

using namespace anh;
using namespace app;
using namespace swganh::login;
using namespace messages;
using namespace network::soe;
using namespace swganh::login;
using namespace swganh::base;
using namespace swganh::character;
using namespace swganh::galaxy;
using namespace database;
using namespace event_dispatcher;
using namespace std;

using anh::network::soe::Session;

LoginService::LoginService(string listen_address, uint16_t listen_port, KernelInterface* kernel)     
    : BaseService(kernel)
#pragma warning(push)
#pragma warning(disable: 4355)
    , SwgMessageRouter([=] (const boost::asio::ip::udp::endpoint& endpoint) {
        return GetClientFromEndpoint(endpoint);  
      })
#pragma warning(pop) 
    , galaxy_status_timer_(kernel->GetIoService())
    , listen_address_(listen_address)
    , listen_port_(listen_port)
     {
    
    soe_server_.reset(new Server(
        kernel->GetIoService(),
        kernel->GetEventDispatcher(),
        bind(&LoginService::RouteMessage, this, placeholders::_1)));
    
    account_provider_ = kernel->GetPluginManager()->CreateObject<providers::AccountProviderInterface>("LoginService::AccountProvider");
    if (!account_provider_) 
    {
        account_provider_ = make_shared<providers::MysqlAccountProvider>(kernel->GetDatabaseManager());
    }

    shared_ptr<encoders::EncoderInterface> encoder = kernel->GetPluginManager()->CreateObject<encoders::EncoderInterface>("LoginService::Encoder");
    if (!encoder) {
        encoder = make_shared<encoders::Sha512Encoder>(kernel->GetDatabaseManager());
    }

    authentication_manager_ = make_shared<AuthenticationManager>(encoder);
}

LoginService::~LoginService() {}

service::ServiceDescription LoginService::GetServiceDescription() {
    service::ServiceDescription service_description(
        "ANH Login Service",
        "login",
        "0.1",
        listen_address_, 
        0, 
        listen_port_, 
        0);

    return service_description;
}

void LoginService::onStart() {
    character_service_ = static_pointer_cast<CharacterService>(kernel()->GetServiceManager()->GetService("CharacterService"));
	galaxy_service_  = static_pointer_cast<GalaxyService>(kernel()->GetServiceManager()->GetService("GalaxyService"));

    soe_server_->Start(listen_port_);
    
    UpdateGalaxyStatus_();
}

void LoginService::onStop() {
    soe_server_->Shutdown();
}

void LoginService::subscribe() {
    auto event_dispatcher = kernel()->GetEventDispatcher();
    
    RegisterMessageHandler<LoginClientId>(
        bind(&LoginService::HandleLoginClientId_, this, placeholders::_1, placeholders::_2), false);

    event_dispatcher->Subscribe(
        "UpdateGalaxyStatus", 
        [this] (const shared_ptr<anh::EventInterface>& incoming_event) 
    {
        UpdateGalaxyStatus_();
    });
    
    event_dispatcher->Subscribe(
        "NetworkSessionRemoved", 
        [this] (const shared_ptr<anh::EventInterface>& incoming_event)
    {
        const auto& session = static_pointer_cast<ValueEvent<shared_ptr<Session>>>(incoming_event)->Get();
        
        // Message was triggered from our server so process it.
        if (session->server() == soe_server_.get()) {
            RemoveClient_(session);
        }
    });
}

int LoginService::galaxy_status_check_duration_secs() const 
{
    return galaxy_status_check_duration_secs_;
}

void LoginService::galaxy_status_check_duration_secs(int new_duration)
{
    galaxy_status_check_duration_secs_ = new_duration;
}

int LoginService::login_error_timeout_secs() const
{
    return login_error_timeout_secs_;
}

void LoginService::login_auto_registration(bool auto_registration)
{
    login_auto_registration_ = auto_registration;
}

bool LoginService::login_auto_registration() const
{
    return login_auto_registration_;
}

void LoginService::login_error_timeout_secs(int new_timeout)
{
    login_error_timeout_secs_ = new_timeout;
}

std::shared_ptr<LoginClient> LoginService::AddClient_(shared_ptr<Session> session) {
    std::shared_ptr<LoginClient> client = GetClientFromEndpoint(session->remote_endpoint());

    if (!client) {
        DLOG(WARNING) << "Adding login client";

        client = make_shared<LoginClient>(session);
        
        ClientMap::accessor a;
        clients_.insert(a, client->GetSession()->remote_endpoint());
        a->second = client;
    }

    DLOG(WARNING) << "Login service currently has ("<< clients_.size() << ") clients";
    return client;
}

void LoginService::RemoveClient_(std::shared_ptr<anh::network::soe::Session> session) {
    std::shared_ptr<LoginClient> client = GetClientFromEndpoint(session->remote_endpoint());
    
    if (client) {
        DLOG(WARNING) << "Removing disconnected client";
        clients_.erase(session->remote_endpoint());
    }

    DLOG(WARNING) << "Login service currently has ("<< clients_.size() << ") clients";
}

void LoginService::UpdateGalaxyStatus_() {    
    DLOG(INFO) << "Updating galaxy status";

    galaxy_status_ = GetGalaxyStatus_();
        
    const vector<GalaxyStatus>& status = galaxy_status_;

    std::for_each(clients_.begin(), clients_.end(), [&status] (ClientMap::value_type& client_entry) {
        if (client_entry.second) {                
            client_entry.second->Send(
                BuildLoginClusterStatus(status));
        }
    });
}

std::vector<GalaxyStatus> LoginService::GetGalaxyStatus_() {
    std::vector<GalaxyStatus> galaxy_status;
    
    auto service_directory = kernel()->GetServiceDirectory();

    auto galaxy_list = service_directory->getGalaxySnapshot();

    std::for_each(galaxy_list.begin(), galaxy_list.end(), [this, &galaxy_status, &service_directory] (anh::service::Galaxy& galaxy) {
        auto service_list = service_directory->getServiceSnapshot(galaxy);

        auto it = std::find_if(service_list.begin(), service_list.end(), [] (anh::service::ServiceDescription& service) {
            return service.type().compare("connection") == 0;
        });

        if (it != service_list.end()) {
            GalaxyStatus status;
            status.address = it->address();
            status.connection_port = it->udp_port();
            // TODO: Add a meaningful value here (ping to client from server?)
            status.distance = 0xffff8f80;
            status.galaxy_id = galaxy.id();
            // TODO: Add a configurable value here
            status.max_characters = 2;
            // TODO: Add a configurable value here
            status.max_population = 0x00000cb2;
            status.name = galaxy.name();
            status.ping_port = it->ping_port();
            status.server_population = galaxy_service_->GetPopulation();
            status.status = service_directory->galaxy().status();

            galaxy_status.push_back(std::move(status));
        }
    });

    return galaxy_status;
}

void LoginService::HandleLoginClientId_(std::shared_ptr<LoginClient> login_client, const LoginClientId& message) {

    login_client->SetUsername(message.username);
    login_client->SetPassword(message.password);
    login_client->SetVersion(message.client_version);
    
    auto account = account_provider_->FindByUsername(message.username);

    if (!account && login_auto_registration_ == true)
    {
        if(account_provider_->AutoRegisterAccount(message.username, message.password))
        {
            account = account_provider_->FindByUsername(message.username); 
        }
    }

    if (!account || !authentication_manager_->Authenticate(login_client, account)) {
        DLOG(WARNING) << "Login request for invalid user: " << login_client->GetUsername();

        ErrorMessage error;
        error.type = "@cpt_login_fail";
        error.message = "@msg_login_fail";
        error.force_fatal = false;
        
        login_client->Send(error);
        
        auto timer = make_shared<boost::asio::deadline_timer>(kernel()->GetIoService(), boost::posix_time::seconds(login_error_timeout_secs_));
        timer->async_wait([login_client] (const boost::system::error_code& e)
        {
			if (login_client)
			{
			    if (login_client->GetSession())
			    {
				    login_client->GetSession()->Close();
			    }
			    
				DLOG(WARNING) << "Closing connection";
			}
            return;            
        });

        return;
    }
    
    login_client->SetAccount(account);
    
    clients_.insert(make_pair(login_client->GetSession()->remote_endpoint(), login_client));
    DLOG(WARNING) << "Login service currently has ("<< clients_.size() << ") clients";
    
    // create account session
    string account_session = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time())
        + boost::lexical_cast<string>(login_client->GetSession()->remote_endpoint().address());

    account_provider_->CreateAccountSession(account->account_id(), account_session);
    login_client->Send(
        messages::BuildLoginClientToken(login_client, account_session));
    
    login_client->Send(
        messages::BuildLoginEnumCluster(login_client, galaxy_status_));
    
    login_client->Send(
        messages::BuildLoginClusterStatus(galaxy_status_));
    
    auto characters = character_service_->GetCharactersForAccount(login_client->GetAccount()->account_id());

    login_client->Send(
        messages::BuildEnumerateCharacterId(characters));
}

uint32_t LoginService::GetAccountBySessionKey(const string& session_key) {
    return account_provider_->FindBySessionKey(session_key);
}


shared_ptr<LoginClient> LoginService::GetClientFromEndpoint(
        const boost::asio::ip::udp::endpoint& remote_endpoint)
{
    shared_ptr<LoginClient> client = nullptr;
    
    ClientMap::accessor a;

    if (clients_.find(a, remote_endpoint)) {
        client = a->second;
    }

    return client;
}
