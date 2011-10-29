
#include "swganh/connection/connection_service.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

#include "anh/crc.h"
#include "anh/event_dispatcher/basic_event.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"
#include "anh/network/soe/packet.h"
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
using namespace anh::network;
using namespace anh::service;
using namespace swganh::base;
using namespace swganh::character;
using namespace swganh::connection;
using namespace swganh::login;
using namespace swganh::messages;
using namespace swganh::simulation;

using namespace std;

using boost::asio::ip::udp;

ConnectionService::ConnectionService(
        string listen_address, 
        uint16_t listen_port, 
        uint16_t ping_port, 
        KernelInterface* kernel)
    : BaseService(kernel)
#pragma warning(push)
#pragma warning(disable: 4355)
    , SwgMessageRouter([=] (const boost::asio::ip::udp::endpoint& endpoint) {
        return GetClientFromEndpoint(endpoint);  
      })
#pragma warning(pop)
    , ping_server_(nullptr)
    , listen_address_(listen_address)
    , listen_port_(listen_port)
    , soe_server_(nullptr)
{
    soe_server_.reset(new soe::Server(
        kernel->GetIoService(),
        bind(&ConnectionService::RouteMessage, this, placeholders::_1)));
    soe_server_->event_dispatcher(kernel->GetEventDispatcher());
        
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
     
    RegisterMessageHandler<ClientIdMsg>(
        bind(&ConnectionService::HandleClientIdMsg_, this, placeholders::_1, placeholders::_2), false);

    RegisterMessageHandler<CmdSceneReady>(
        bind(&ConnectionService::HandleCmdSceneReady_, this, placeholders::_1, placeholders::_2));

    event_dispatcher->subscribe("NetworkSessionRemoved", [this] (shared_ptr<EventInterface> incoming_event) -> bool {
        auto session_removed = std::static_pointer_cast<anh::event_dispatcher::BasicEvent<anh::network::soe::SessionData>>(incoming_event);
        
        // Message was triggered from our server so process it.
        if (session_removed->session->server() == server().get()) {
            RemoveClient_(session_removed->session);
        }

        return true;
    });
}

void ConnectionService::onStart() {
    ping_server_ = make_shared<PingServer>(kernel()->GetIoService(), ping_port_);
    soe_server_->Start(listen_port_);
    
    character_service_ = std::static_pointer_cast<CharacterService>(kernel()->GetServiceManager()->GetService("CharacterService"));    
    login_service_ = std::static_pointer_cast<swganh::login::LoginService>(kernel()->GetServiceManager()->GetService("LoginService"));
    simulation_service_ = std::static_pointer_cast<swganh::simulation::SimulationService>(kernel()->GetServiceManager()->GetService("SimulationService"));
}

void ConnectionService::onStop() {
    soe_server_->Shutdown();
    clients_.clear();
}

const string& ConnectionService::listen_address() {
    return listen_address_;
}

uint16_t ConnectionService::listen_port() {
    return listen_port_;
}

ConnectionService::ClientMap& ConnectionService::clients() {
    return clients_;
}

std::unique_ptr<anh::network::soe::Server>& ConnectionService::server() {
    return soe_server_;
}

shared_ptr<CharacterService> ConnectionService::character_service() {
    return character_service_.lock();
}

shared_ptr<LoginService> ConnectionService::login_service() {
    return login_service_.lock();
}

shared_ptr<ConnectionClient> ConnectionService::GetClientFromEndpoint(
        const udp::endpoint& remote_endpoint)
{
    shared_ptr<ConnectionClient> client = nullptr;
    
    ClientMap::accessor a;

    if (clients_.find(a, remote_endpoint)) {
        client = a->second;
    }

    return client;
}

void ConnectionService::AddClient_(
    uint64_t player_id, 
    std::shared_ptr<ConnectionClient> client) 
{
    ClientMap& client_map = clients();

    auto find_it = std::find_if(
        client_map.begin(), 
        client_map.end(), 
        [client, player_id] (ClientMap::value_type& conn_client) 
    {
        return conn_client.second->GetPlayerId() == player_id;
    });

    if (find_it != client_map.end()) {
        LogoutMessage message;
        (*find_it).second->Send(message);

        client_map.erase(find_it->first);
    }
    
    DLOG(WARNING) << "Adding connection client";

    ClientMap::accessor a;
    client_map.insert(a, client->GetSession()->remote_endpoint());
    a->second = client;

    DLOG(WARNING) << "Connection service currently has ("<< client_map.size() << ") clients";
}

void ConnectionService::RemoveClient_(std::shared_ptr<anh::network::soe::Session> session) {
    auto client = GetClientFromEndpoint(session->remote_endpoint());
    
    auto client_map = clients();

    if (client) {

        auto controller = client->GetController();
        if (controller)
        {
			// player is always + 1 from the creature
			auto simulation_service = simulation_service_.lock();
            auto player = simulation_service->GetObjectById<swganh::object::player::Player>(controller->GetObject()->GetObjectId() + 1);
			player->AddStatusFlag(swganh::object::player::LD);
			
			// set a timer to 5 minutes to destroy the object, unless logged back in.
			auto deadline_timer = make_shared<boost::asio::deadline_timer>(kernel()->GetIoService(), boost::posix_time::seconds(30));
			deadline_timer->async_wait(boost::bind(&ConnectionService::RemoveClientTimerHandler_, this, boost::asio::placeholders::error, deadline_timer, 10, controller));
        }

        DLOG(WARNING) << "Removing disconnected client";
        client_map.erase(session->remote_endpoint());
		DLOG(WARNING) << "Removing Session";
		session_provider_->EndGameSession(client->GetPlayerId());
    }
            
    DLOG(WARNING) << "Connection service currently has ("<< client_map.size() << ") clients";
}
void ConnectionService::RemoveClientTimerHandler_(const boost::system::error_code& e, shared_ptr<boost::asio::deadline_timer> timer, int delay_in_secs, shared_ptr<swganh::object::ObjectController> controller)
{
	if (controller)
	{
		// destroy if they haven't reconnected
		if (controller->GetRemoteClient()->GetSession() == nullptr || !controller->GetRemoteClient()->GetSession()->connected())
		{
			auto object_id = controller->GetObject()->GetObjectId();
			DLOG(WARNING) << "Destroying Object " << object_id << " after " << delay_in_secs << " seconds.";
			auto simulation_service = simulation_service_.lock();
			simulation_service->RemoveObjectById(object_id);
		}
	}
}
void ConnectionService::HandleCmdSceneReady_(std::shared_ptr<ConnectionClient> client, const CmdSceneReady& message) {
    DLOG(WARNING) << "Handling CmdSceneReady";
    
    client->Send(CmdSceneReady());
}

void ConnectionService::HandleClientIdMsg_(std::shared_ptr<ConnectionClient> client, const ClientIdMsg& message) {
    DLOG(WARNING) << "Handling ClientIdMsg";

    // get session key from login service
    uint32_t account_id = login_service()->GetAccountBySessionKey(message.session_hash);

    // authorized
    if (! account_id) {
        DLOG(WARNING) << "Account_id not found from session key, unauthorized access.";
        return;
    }
    
    // gets player from account
    uint64_t player_id = session_provider_->GetPlayerId(account_id);
                
    // authorized
    if (! player_id) {
        DLOG(WARNING) << "No player found for the requested account, unauthorized access.";
        return;
    }
    
    // creates a new session and stores it for later use
    if (!session_provider_->CreateGameSession(player_id, client->GetSession()->connection_id())) {    
        DLOG(WARNING) << "Player Not Inserted into Session Map because No Game Session Created!";
    }
    
    client->Connect(account_id, player_id);

    AddClient_(player_id, client);

    ClientPermissionsMessage client_permissions;
    client_permissions.galaxy_available = kernel()->GetServiceDirectory()->galaxy().status();
    client_permissions.available_character_slots = character_service()->GetMaxCharacters(account_id);
    // @TODO: Replace with configurable value
    client_permissions.unlimited_characters = 0;

    client->Send(client_permissions);
}
