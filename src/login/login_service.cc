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

#include "login/login_service.h"

#include <glog/logging.h>

#include "anh/app/kernel_interface.h"

#include "anh/database/database_manager.h"

#include "anh/event_dispatcher/basic_event.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/session.h"
#include "anh/network/soe/server.h"

#include "anh/service/service_manager.h"

#include "swganh/base/swg_message_handler.h"

#include "login/messages/delete_character_message.h"
#include "login/messages/delete_character_reply_message.h"
#include "login/messages/enumerate_character_id.h"
#include "login/messages/login_client_id.h"
#include "login/messages/login_client_token.h"
#include "login/messages/login_cluster_status.h"
#include "login/messages/login_enum_cluster.h"

#include "login/authentication_manager.h"
#include "login/login_client.h"
#include "login/encoders/sha512_encoder.h"
#include "login/providers/mysql_account_provider.h"

using namespace anh;
using namespace app;
using namespace swganh;
using namespace base;
using namespace character;
using namespace database;
using namespace event_dispatcher;
using namespace login;
using namespace messages;
using namespace std;

LoginService::LoginService(shared_ptr<KernelInterface> kernel) 
    : swganh::base::BaseService(kernel)
    , listen_port_(0) 
    , galaxy_status_timer_(kernel->GetIoService()) {
    soe_server_.reset(new network::soe::Server(swganh::base::SwgMessageHandler(kernel->GetEventDispatcher())));
    
    auto encoder = make_shared<encoders::Sha512Encoder>(kernel->GetDatabaseManager());

    authentication_manager_ = make_shared<AuthenticationManager>(encoder);
    account_provider_ = make_shared<providers::MysqlAccountProvider>(kernel->GetDatabaseManager());
}

LoginService::~LoginService() {}

service::Service LoginService::GetServiceDescription() {
    service::Service service_description(
        "ANH Login Service",
        "login",
        "0.1",
        listen_address_, 
        0, 
        listen_port_, 
        0);

    return service_description;
}

void LoginService::DescribeConfigOptions(boost::program_options::options_description& description) {
    description.add_options()
        ("service.login.udp_port", boost::program_options::value<uint16_t>(&listen_port_),
            "The port the login service will listen for incoming client connections on")
        ("service.login.address", boost::program_options::value<string>(&listen_address_),
            "The public address the login service will listen for incoming client connections on")
        ("service.login.status_check_duration_secs", boost::program_options::value<int>(&galaxy_status_check_duration_secs_),
            "The amount of time between checks for updated galaxy status")
    ;
}

void LoginService::onStart() {
    character_service_ = static_pointer_cast<BaseCharacterService>(kernel()->GetServiceManager()->GetService("CharacterService"));

    soe_server_->Start(listen_port_);
    galaxy_status_ = GetGalaxyStatus_();
    
    UpdateGalaxyStatus_();
}

void LoginService::onUpdate() {
    soe_server_->Update();    

    // Add timer to update galaxy status every X configurable seconds
}

void LoginService::onStop() {
    soe_server_->Shutdown();
}

void LoginService::subscribe() {
    auto event_dispatcher = kernel()->GetEventDispatcher();

    event_dispatcher->subscribe("LoginClientId", bind(&LoginService::HandleLoginClientId_, this, placeholders::_1));
    event_dispatcher->subscribe("DeleteCharacterMessage", bind(&LoginService::HandleDeleteCharacterMessage_, this, placeholders::_1));
    event_dispatcher->subscribe("GalaxyStatusUpdated", bind(&LoginService::HandleGalaxyStatusUpdated_, this, placeholders::_1));
}

void LoginService::UpdateGalaxyStatus_() {    
    galaxy_status_timer_.expires_from_now(boost::posix_time::seconds(galaxy_status_check_duration_secs_));
    galaxy_status_timer_.async_wait(strand().wrap([this] (const boost::system::error_code & error) {
        galaxy_status_ = GetGalaxyStatus_();
        
        kernel()->GetEventDispatcher()->triggerAsync(make_shared_event("GalaxyStatusUpdated"));

        if (IsRunning()) {
            UpdateGalaxyStatus_();
        }
    }));
}

std::vector<GalaxyStatus> LoginService::GetGalaxyStatus_() {
    std::vector<GalaxyStatus> galaxy_status;
    
    auto service_directory = this->service_directory();

    auto galaxy_list = service_directory->getGalaxySnapshot();

    std::for_each(galaxy_list.begin(), galaxy_list.end(), [this, &galaxy_status, &service_directory] (anh::service::Galaxy& galaxy) {
        auto service_list = service_directory->getServiceSnapshot(std::make_shared<anh::service::Galaxy>(galaxy));

        auto it = std::find_if(service_list.begin(), service_list.end(), [] (anh::service::Service& service) {
            return service.type().compare("connection") == 0;
        });

        if (it != service_list.end()) {
            GalaxyStatus status;
            status.address = it->address();
            status.connection_port = it->udp_port();
            status.distance = 0xffff8f80;
            status.galaxy_id = galaxy.id();
            status.max_characters = 2;
            status.max_population = 0x00000cb2;
            status.name = galaxy.name();
            status.ping_port = it->ping_port();
            status.server_population = 10;
            status.status = it->status();

            galaxy_status.push_back(std::move(status));
        }
    });

    return galaxy_status;
}

bool LoginService::HandleGalaxyStatusUpdated_(shared_ptr<EventInterface> incoming_event) {
    DLOG(WARNING) << "Sending out galaxy status update";

    std::for_each(clients_.begin(), clients_.end(), [this] (ClientMap::value_type& client_entry) {
        if (client_entry.second) {
            client_entry.second->session->SendMessage(
                BuildLoginClusterStatus(galaxy_status_));
        }
    });

    return true;
}

bool LoginService::HandleLoginClientId_(shared_ptr<EventInterface> incoming_event) {
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
    LoginClientId message;
    message.deserialize(*remote_event->message());

    auto login_client = make_shared<LoginClient>();

    login_client->username = message.username;
    login_client->password = message.password;
    login_client->version = message.client_version;

    auto account = account_provider_->FindByUsername(login_client->username);

    if (! account) {
        DLOG(WARNING) << "Login request for invalid user: " << login_client->username;
        return true;
    }

    if (! authentication_manager_->Authenticate(login_client, account)) {
        DLOG(WARNING) << "Failed login attempt for user: " << login_client->username;
        return true;
    }

    login_client->account = account;
    login_client->session = remote_event->session();

    login_client->session->SendMessage(
        messages::BuildLoginClientToken(login_client));
    
    login_client->session->SendMessage(
        messages::BuildLoginEnumCluster(login_client, galaxy_status_));
    
    login_client->session->SendMessage(
        messages::BuildLoginClusterStatus(galaxy_status_));
    

    auto characters = character_service_->GetCharactersForAccount(login_client->account->account_id());

    login_client->session->SendMessage(
        messages::BuildEnumerateCharacterId(characters));

    clients_.insert(make_pair(login_client->account->account_id(), login_client));

    return true;
}

bool LoginService::HandleDeleteCharacterMessage_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event) {
    auto remote_event = static_pointer_cast<BasicEvent<anh::network::soe::Packet>>(incoming_event);
    
    DeleteCharacterMessage message;
    message.deserialize(*remote_event->message());

    DeleteCharacterReplyMessage reply_message;
    reply_message.failure_flag = 1;

    if (character_service_->DeleteCharacter(message.character_id)) {
        reply_message.failure_flag = 0;
    }

    remote_event->session()->SendMessage(reply_message);

    return true;
}
