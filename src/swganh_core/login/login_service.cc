// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "login_service.h"

#include "swganh/logger.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "swganh/database/database_manager.h"

#include "swganh/event_dispatcher.h"

#include "swganh/network/soe/session.h"
#include "swganh/network/soe/server.h"

#include "swganh/service/service_directory_interface.h"
#include "swganh/service/service_manager.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/character/character_provider_interface.h"

#include "swganh_core/messages/enumerate_character_id.h"
#include "swganh_core/messages/error_message.h"
#include "swganh_core/messages/login_client_token.h"
#include "swganh_core/messages/login_cluster_status.h"
#include "swganh_core/messages/login_enum_cluster.h"

#include "authentication_manager.h"
#include "login_client.h"
#include "swganh_core/login/providers/account_provider_interface.h"
#include "swganh_core/login/encoders/encoder_interface.h"

using namespace swganh;
using namespace app;
using namespace swganh::login;
using namespace swganh::login;
using namespace swganh::messages;
using namespace network::soe;
using namespace swganh::login;
using namespace swganh::character;
using namespace swganh::galaxy;
using namespace database;
using namespace event_dispatcher;
using namespace std;

using boost::asio::ip::udp;
using swganh::app::SwganhKernel;

LoginService::LoginService(string listen_address, uint16_t listen_port, SwganhKernel* kernel)
    : swganh::login::LoginServiceInterface(kernel->GetCpuThreadPool())
    , kernel_(kernel)
	, galaxy_status_timer_(kernel->GetCpuThreadPool())
    , listen_address_(listen_address)
    , listen_port_(listen_port)
	, active_(kernel->GetCpuThreadPool())
{
    account_provider_ = kernel->GetPluginManager()->CreateObject<swganh::login::providers::AccountProviderInterface>("Login::AccountProvider");
    
    shared_ptr<encoders::EncoderInterface> encoder = kernel->GetPluginManager()->CreateObject<encoders::EncoderInterface>("Login::Encoder");

    character_provider_ = kernel->GetPluginManager()->CreateObject<CharacterProviderInterface>("Character::CharacterProvider");

    authentication_manager_ = make_shared<AuthenticationManager>(encoder);
}

LoginService::~LoginService() {
    session_timer_->cancel();
    session_timer_.reset();
}

service::ServiceDescription LoginService::GetServiceDescription() {
    auto listen_address = Resolve(listen_address_);

    service::ServiceDescription service_description(
        "Login Service",
        "login",
        "0.1",
        listen_address,
        0,
        listen_port_,
        0);

    return service_description;
}

shared_ptr<Session> LoginService::CreateSession(const udp::endpoint& endpoint)
{
    shared_ptr<LoginClient> session = nullptr;

    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        if (session_map_.find(endpoint) == session_map_.end())
        {
            session = make_shared<LoginClient>(this, kernel_->GetCpuThreadPool(), endpoint);
            session_map_.insert(make_pair(endpoint, session));
        }
    }

    return session;
}

bool LoginService::RemoveSession(std::shared_ptr<Session> session) {
    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        session_map_.erase(session->remote_endpoint());
    }

    return true;
}

shared_ptr<Session> LoginService::GetSession(const udp::endpoint& endpoint) {
    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);

        auto find_iter = session_map_.find(endpoint);
        if (find_iter != session_map_.end())
        {
            return find_iter->second;
        }
    }

    return CreateSession(endpoint);
}

void LoginService::Startup() {
    character_service_ = kernel_->GetServiceManager()->GetService<CharacterServiceInterface>("CharacterService");
	galaxy_service_  = kernel_->GetServiceManager()->GetService<GalaxyServiceInterface>("GalaxyService");
    
    RegisterMessageHandler(&LoginService::HandleLoginClientId_, this);

    auto event_dispatcher = kernel_->GetEventDispatcher();

    event_dispatcher->Subscribe(
        "UpdateGalaxyStatus",
        [this] (const shared_ptr<swganh::EventInterface>& incoming_event)
    {
        UpdateGalaxyStatus_();
    });

    Server::Startup(listen_port_);

    UpdateGalaxyStatus_();

    session_timer_ = active_.AsyncRepeated(boost::posix_time::milliseconds(5), [this] () {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        for_each(
            begin(session_map_),
            end(session_map_),
            [=] (SessionMap::value_type& type)
        {
            type.second->Update();
        });
    });
}

void LoginService::Shutdown()
{
    // Remove all the sessions
    account_provider_->EndSessions();
    Server::Shutdown();
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

void LoginService::UpdateGalaxyStatus_() {
    //LOG(info) << "Updating galaxy status";

    galaxy_status_ = GetGalaxyStatus_();

    auto status_message = BuildLoginClusterStatus(galaxy_status_);

    boost::lock_guard<boost::mutex> lg(session_map_mutex_);
    std::for_each(
        begin(session_map_),
        end(session_map_),
        [&status_message] (SessionMap::value_type& item)
    {
        if (item.second) {
            item.second->SendTo(status_message);
        }
    });
}

std::vector<GalaxyStatus> LoginService::GetGalaxyStatus_() {
    std::vector<GalaxyStatus> galaxy_status;

    auto service_directory = kernel_->GetServiceDirectory();

    auto galaxy_list = service_directory->getGalaxySnapshot();

    std::for_each(galaxy_list.begin(), galaxy_list.end(), [this, &galaxy_status, &service_directory] (swganh::service::Galaxy& galaxy) {
        auto service_list = service_directory->getServiceSnapshot(galaxy);

        auto it = std::find_if(service_list.begin(), service_list.end(), [] (swganh::service::ServiceDescription& service) {
            return service.type().compare("connection") == 0;
        });

        if (it != service_list.end()) {
            GalaxyStatus status;
            status.address = it->address();
            status.connection_port = it->udp_port();
            //@TODO: Add a meaningful value here (ping to client from server?)
            status.distance = 0xffff8f80;
            status.galaxy_id = galaxy.id();
            //@TODO: Add a configurable value here
            status.max_characters = 2;
            //@TODO: Add a configurable value here
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

void LoginService::HandleLoginClientId_(const std::shared_ptr<LoginClientInterface>& login_client, LoginClientId* message)
{
    login_client->SetUsername(message->username);
    login_client->SetPassword(message->password);
    login_client->SetVersion(message->client_version);

    auto account = account_provider_->FindByUsername(message->username);

    if (!account && login_auto_registration_ == true)
    {
        if(account_provider_->AutoRegisterAccount(message->username, message->password))
        {
            account = account_provider_->FindByUsername(message->username);
        }
    }

    if (!account || !authentication_manager_->Authenticate(login_client, account)) {
        LOG(warning) << "Login request for invalid user: " << login_client->GetUsername();

        ErrorMessage error;
        error.type = "@cpt_login_fail";
        error.message = "@msg_login_fail";
        error.force_fatal = false;

        login_client->SendTo(error);

        auto timer = std::make_shared<boost::asio::deadline_timer>(kernel_->GetCpuThreadPool(), boost::posix_time::seconds(login_error_timeout_secs_));
        timer->async_wait([login_client] (const boost::system::error_code& e)
        {
			if (login_client)
			{
                login_client->Close();

				DLOG(info) << "Closing connection";
			}
        });

        return;
    }

    login_client->SetAccount(account);
    // create account session
    string account_session = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time())
        + boost::lexical_cast<string>(login_client->remote_endpoint().address());

    account_provider_->CreateAccountSession(account->account_id(), account_session);
    login_client->SendTo(
        BuildLoginClientToken(login_client, account_session));

    login_client->SendTo(
        BuildLoginEnumCluster(login_client, galaxy_status_));

    login_client->SendTo(
        BuildLoginClusterStatus(galaxy_status_));

    auto characters = character_provider_->GetCharactersForAccount(login_client->GetAccount()->account_id());

    login_client->SendTo(
        BuildEnumerateCharacterId(characters));
}

uint32_t LoginService::GetAccountBySessionKey(const string& session_key) {
    return account_provider_->FindBySessionKey(session_key);
}
