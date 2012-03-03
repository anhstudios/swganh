
#include "swganh/connection/connection_service.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

#include "anh/crc.h"
#include "anh/event_dispatcher.h"
#include "anh/network/soe/server.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_directory_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/connection/ping_server.h"
#include "swganh/connection/connection_client.h"
#include "swganh/connection/providers/mysql_session_provider.h"

#include "swganh/object/object.h"
#include "swganh/object/player/player.h"

#include "swganh/messages/logout_message.h"

using namespace anh::app;
using namespace anh::event_dispatcher;
using namespace anh::network::soe;
using namespace anh::service;
using namespace swganh::base;
using namespace swganh::character;
using namespace swganh::connection;
using namespace swganh::login;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::simulation;

using namespace std;

using anh::ValueEvent;

using boost::asio::ip::udp;

ConnectionService::ConnectionService(
        string listen_address, 
        uint16_t listen_port, 
        uint16_t ping_port, 
        KernelInterface* kernel)
    : BaseService(kernel)
    , swganh::network::BaseSwgServer(kernel->GetIoService())
    , ping_server_(nullptr)
    , listen_address_(listen_address)
    , listen_port_(listen_port)
    , ping_port_(ping_port)
{
        
    session_provider_ = kernel->GetPluginManager()->CreateObject<providers::SessionProviderInterface>("ConnectionService::SessionProvider");
    if (!session_provider_) 
    {
        session_provider_ = make_shared<providers::MysqlSessionProvider>(kernel->GetDatabaseManager());
    }
}

ServiceDescription ConnectionService::GetServiceDescription() {
    ServiceDescription service_description(
        "ANH Connection Service",
        "connection",
        "0.1",
        listen_address(), 
        0,
        listen_port(), 
        ping_port_);

    return service_description;
}

void ConnectionService::subscribe() {
    auto event_dispatcher = kernel()->GetEventDispatcher();
     
    RegisterMessageHandler(&ConnectionService::HandleClientIdMsg_, this);
    RegisterMessageHandler(&ConnectionService::HandleCmdSceneReady_, this);
}

void ConnectionService::onStart() {
    ping_server_ = make_shared<PingServer>(kernel()->GetIoService(), ping_port_);

    character_service_ = std::static_pointer_cast<CharacterService>(kernel()->GetServiceManager()->GetService("CharacterService"));    
    login_service_ = std::static_pointer_cast<swganh::login::LoginService>(kernel()->GetServiceManager()->GetService("LoginService"));
    simulation_service_ = std::static_pointer_cast<swganh::simulation::SimulationService>(kernel()->GetServiceManager()->GetService("SimulationService"));
    
    Server::Start(listen_port_);
    
    active().AsyncRepeated(boost::posix_time::milliseconds(5), [this] () {
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

void ConnectionService::onStop() {
    Server::Shutdown();
}

const string& ConnectionService::listen_address() {
    return listen_address_;
}

uint16_t ConnectionService::listen_port() {
    return listen_port_;
}

shared_ptr<Session> ConnectionService::CreateSession(const udp::endpoint& endpoint)
{
    shared_ptr<ConnectionClient> session = nullptr;

    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        if (session_map_.find(endpoint) == session_map_.end())
        {
            session = make_shared<ConnectionClient>(endpoint, this);
            session_map_.insert(make_pair(endpoint, session));
        }
    }

    return session;
}

bool ConnectionService::RemoveSession(std::shared_ptr<Session> session) {
    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);
        session_map_.erase(session->remote_endpoint());
    }

    auto connection_client = static_pointer_cast<ConnectionClient>(session);
    
    auto controller = connection_client->GetController();
    if (controller)
    {
        auto simulation_service = simulation_service_.lock();

        // @TODO REFACTOR Move this functionality out to a PlayerService
        auto player = simulation_service->GetObjectById<swganh::object::player::Player>(controller->GetObject()->GetObjectId() + 1);
		player->AddStatusFlag(swganh::object::player::LD);
        // END TODO

        simulation_service->PersistRelatedObjects(controller->GetObject()->GetObjectId());
        
        // set a timer to 5 minutes to destroy the object, unless logged back in.
        auto deadline_timer = std::make_shared<boost::asio::deadline_timer>(kernel()->GetIoService(), boost::posix_time::seconds(30));
        deadline_timer->async_wait(boost::bind(&ConnectionService::RemoveClientTimerHandler_, this, boost::asio::placeholders::error, deadline_timer, 10, controller));
    }

    BOOST_LOG_TRIVIAL(info) << "Removing disconnected client";
    session_provider_->EndGameSession(connection_client->GetPlayerId());

    return true;
}

shared_ptr<Session> ConnectionService::GetSession(const udp::endpoint& endpoint) {
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

std::shared_ptr<ConnectionClient> ConnectionService::FindConnectionByPlayerId(uint64_t player_id)
{    
    shared_ptr<ConnectionClient> connection = nullptr;

    {
        boost::lock_guard<boost::mutex> lg(session_map_mutex_);

        auto find_iter = find_if(
            begin(session_map_),
            end(session_map_),
            [player_id] (SessionMap::value_type& item)
        {
            return item.second->GetPlayerId() == player_id;
        });

        if (find_iter != end(session_map_))
        {
            connection = find_iter->second;
        }
    }

    return connection;
}

shared_ptr<CharacterService> ConnectionService::character_service() {
    return character_service_.lock();
}

shared_ptr<LoginService> ConnectionService::login_service() {
    return login_service_.lock();
}

void ConnectionService::RemoveClientTimerHandler_(
    const boost::system::error_code& e, 
    shared_ptr<boost::asio::deadline_timer> timer, 
    int delay_in_secs, 
    shared_ptr<swganh::object::ObjectController> controller)
{
    if (controller)
    {
        // destroy if they haven't reconnected
        if (controller->GetRemoteClient() == nullptr || !controller->GetRemoteClient()->connected())
        {
            auto object = controller->GetObject();
            BOOST_LOG_TRIVIAL(warning) << "Destroying Object " << object->GetObjectId() << " after " << delay_in_secs << " seconds.";
            auto simulation_service = simulation_service_.lock();
            simulation_service->RemoveObject(object);

            kernel()->GetEventDispatcher()->Dispatch(
                make_shared<ValueEvent<shared_ptr<Object>>>("ObjectRemovedEvent", object));
        }
    }
}

void ConnectionService::HandleCmdSceneReady_(const std::shared_ptr<ConnectionClient>& client, const CmdSceneReady& message) {
    BOOST_LOG_TRIVIAL(warning) << "Handling CmdSceneReady";    

    client->SendMessage(CmdSceneReady());

    auto object = client->GetController()->GetObject();

    kernel()->GetEventDispatcher()->Dispatch(
        make_shared<ValueEvent<shared_ptr<Object>>>("ObjectReadyEvent", object));
}

void ConnectionService::HandleClientIdMsg_(const std::shared_ptr<ConnectionClient>& client, const ClientIdMsg& message) {
    BOOST_LOG_TRIVIAL(warning) << "Handling ClientIdMsg";

    // get session key from login service
    uint32_t account_id = login_service()->GetAccountBySessionKey(message.session_hash);

    // authorized
    if (! account_id) {
        BOOST_LOG_TRIVIAL(warning) << "Account_id not found from session key, unauthorized access.";
        return;
    }
    
    // gets player from account
    uint64_t player_id = session_provider_->GetPlayerId(account_id);
                
    // authorized
    if (! player_id) {
        BOOST_LOG_TRIVIAL(warning) << "No player found for the requested account, unauthorized access.";
        return;
    }

    auto existing_session_connection = FindConnectionByPlayerId(player_id);
    if (existing_session_connection)
    {
        existing_session_connection->Close();
    }
    
    // creates a new session and stores it for later use
    if (!session_provider_->CreateGameSession(player_id, client->connection_id())) {    
        BOOST_LOG_TRIVIAL(warning) << "Player Not Inserted into Session Map because No Game Session Created!";
    }
    
    client->Connect(account_id, player_id);

    ClientPermissionsMessage client_permissions;
    client_permissions.galaxy_available = kernel()->GetServiceDirectory()->galaxy().status();
    client_permissions.available_character_slots = character_service()->GetMaxCharacters(account_id);
    // @TODO: Replace with configurable value
    client_permissions.unlimited_characters = 0;

    client->SendMessage(client_permissions);
}
