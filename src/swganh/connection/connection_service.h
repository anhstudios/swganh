
#ifndef SWGANH_CONNECTION_CONNECTION_SERVICE_H_
#define SWGANH_CONNECTION_CONNECTION_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include <tbb/concurrent_hash_map.h>

#include "anh/hash_string.h"

#include "anh/network/soe/packet.h"
#include "anh/network/soe/packet_utilities.h"
#include "anh/network/soe/session.h"

#include "swganh/base/base_service.h"
#include "swganh/base/swg_message_router.h"

#include "swganh/character/character_service.h"
#include "swganh/login/login_service.h"
#include "swganh/simulation/simulation_service.h"
#include "swganh/messages/cmd_scene_ready.h"

#include "swganh/connection/providers/session_provider_interface.h"

#include "swganh/messages/client_permissions_message.h"
#include "swganh/messages/client_id_msg.h"
#include "swganh/messages/heart_beat.h"

namespace anh {
namespace network {
namespace soe {
class Server;
}}}  // namespace anh::network::soe

namespace swganh {
namespace connection {
    
class PingServer;
class ConnectionClient;
    
class ConnectionService 
    : public swganh::base::BaseService
    , public swganh::base::SwgMessageRouter<ConnectionClient>
{
public:
    typedef tbb::concurrent_hash_map<
        boost::asio::ip::udp::endpoint, 
        std::shared_ptr<ConnectionClient>,
        anh::network::soe::EndpointHashCompare
    > ClientMap;
public:
    ConnectionService(
        std::string listen_address, 
        uint16_t listen_port, 
        uint16_t ping_port, 
        anh::app::KernelInterface* kernel);

    anh::service::ServiceDescription GetServiceDescription();

    void subscribe();

    void onStart();
    void onStop();
    
    std::shared_ptr<ConnectionClient> GetClientFromEndpoint(
        const boost::asio::ip::udp::endpoint& remote_endpoint);

protected:
    const std::string& listen_address();

    uint16_t listen_port();

    ClientMap& clients();

    std::unique_ptr<anh::network::soe::Server>& server();
    
    std::shared_ptr<swganh::character::CharacterService> character_service();

    std::shared_ptr<swganh::login::LoginService> login_service();

private:    
    ClientMap clients_;
    
    void HandleClientIdMsg_(
        std::shared_ptr<swganh::connection::ConnectionClient> client, 
        const swganh::messages::ClientIdMsg& message);
    void HandleCmdSceneReady_(
        std::shared_ptr<swganh::connection::ConnectionClient> client, 
        const swganh::messages::CmdSceneReady& message);
    
    void RemoveClient_(std::shared_ptr<anh::network::soe::Session> session);
	// Async Remove
	void RemoveClientTimerHandler_(const boost::system::error_code& e, std::shared_ptr<boost::asio::deadline_timer> timer, int delay_in_secs, std::shared_ptr<swganh::object::ObjectController> controller);
    void AddClient_(uint64_t player_id, std::shared_ptr<swganh::connection::ConnectionClient> client);

    std::shared_ptr<PingServer> ping_server_;
    std::shared_ptr<providers::SessionProviderInterface> session_provider_;
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    
    std::weak_ptr<swganh::character::CharacterService> character_service_;
    std::weak_ptr<swganh::login::LoginService> login_service_;
    std::weak_ptr<swganh::simulation::SimulationService> simulation_service_;

    std::string listen_address_;
    uint16_t listen_port_;
    uint16_t ping_port_;
};
    
}}  // namespace swganh::connection

#endif  // SWGANH_CONNECTION_CONNECTION_SERVICE_H_
