// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CONNECTION_CONNECTION_SERVICE_H_
#define SWGANH_CONNECTION_CONNECTION_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/thread/mutex.hpp>

#include "anh/active_object.h"
#include "anh/hash_string.h"

#include "anh/network/soe/packet_utilities.h"
#include "anh/network/soe/session.h"
#include "anh/service/service_interface.h"

#include "swganh/network/base_swg_server.h"

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
namespace character {
class CharacterProviderInterface;
class CharacterService;
}}

namespace swganh {
namespace connection {
    
class PingServer;
class ConnectionClient;
    
class ConnectionService 
    : public anh::service::ServiceInterface
    , public swganh::network::BaseSwgServer
{
public:
    ConnectionService(
        std::string listen_address, 
        uint16_t listen_port, 
        uint16_t ping_port, 
        swganh::app::SwganhKernel* kernel);
    
    ~ConnectionService();

    anh::service::ServiceDescription GetServiceDescription();
    
    bool RemoveSession(std::shared_ptr<anh::network::soe::Session> session);

    std::shared_ptr<anh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint);
    
    std::shared_ptr<ConnectionClient> FindConnectionByPlayerId(uint64_t player_id);
    
    void Start();
    void Stop();

    const std::string& listen_address();

    uint16_t listen_port();
        
private:        
    std::shared_ptr<anh::network::soe::Session> CreateSession(const boost::asio::ip::udp::endpoint& endpoint);
    
    void RemoveClientTimerHandler_(
        const boost::system::error_code& e, 
        std::shared_ptr<boost::asio::deadline_timer> timer, 
        int delay_in_secs, 
        std::shared_ptr<swganh::object::ObjectController> controller);

    void HandleClientIdMsg_(
        const std::shared_ptr<ConnectionClient>& client, 
        swganh::messages::ClientIdMsg message);

    void HandleCmdSceneReady_(
        const std::shared_ptr<ConnectionClient>& client, 
        swganh::messages::CmdSceneReady message);
   
    typedef std::map<
        boost::asio::ip::udp::endpoint,
        std::shared_ptr<ConnectionClient>
    > SessionMap;
    
    boost::mutex session_map_mutex_;
    SessionMap session_map_;

    swganh::app::SwganhKernel* kernel_;
    std::shared_ptr<PingServer> ping_server_;
    
    std::shared_ptr<providers::SessionProviderInterface> session_provider_;
    std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;

    swganh::character::CharacterService* character_service_;
    swganh::login::LoginService* login_service_;
    swganh::simulation::SimulationService* simulation_service_;

    anh::ActiveObject active_;
    std::string listen_address_;
    uint16_t listen_port_;
    uint16_t ping_port_;
    std::shared_ptr<boost::asio::deadline_timer> session_timer_;
};
    
}}  // namespace swganh::connection

#endif  // SWGANH_CONNECTION_CONNECTION_SERVICE_H_
