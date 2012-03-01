
#include "swganh/connection/connection_service.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>

#include "anh/crc.h"
#include "anh/event_dispatcher.h"
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
    
    character_service_ = std::static_pointer_cast<CharacterService>(kernel()->GetServiceManager()->GetService("CharacterService"));    
    login_service_ = std::static_pointer_cast<swganh::login::LoginService>(kernel()->GetServiceManager()->GetService("LoginService"));
    simulation_service_ = std::static_pointer_cast<swganh::simulation::SimulationService>(kernel()->GetServiceManager()->GetService("SimulationService"));
    
    Server::Start(listen_port_);
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
    return make_shared<ConnectionClient>(endpoint, this);
}

shared_ptr<CharacterService> ConnectionService::character_service() {
    return character_service_.lock();
}

shared_ptr<LoginService> ConnectionService::login_service() {
    return login_service_.lock();
}

void ConnectionService::HandleCmdSceneReady_(std::shared_ptr<ConnectionClient> client, const CmdSceneReady& message) {
    BOOST_LOG_TRIVIAL(warning) << "Handling CmdSceneReady";    

    client->SendMessage(CmdSceneReady());

    auto object = client->GetController()->GetObject();

    kernel()->GetEventDispatcher()->Dispatch(
        make_shared<ValueEvent<shared_ptr<Object>>>("ObjectReadyEvent", object));
}

void ConnectionService::HandleClientIdMsg_(std::shared_ptr<ConnectionClient> client, const ClientIdMsg& message) {
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
