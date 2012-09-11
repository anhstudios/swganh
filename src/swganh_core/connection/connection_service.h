// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/thread/mutex.hpp>

#include "swganh/connection/connection_service_interface.h"

namespace swganh {
namespace connection {

class PingServer;

class ConnectionService : public swganh::connection::ConnectionServiceInterface
{
public:
    ConnectionService(
        std::string listen_address, 
        uint16_t listen_port, 
        uint16_t ping_port, 
        swganh::app::SwganhKernel* kernel);
    
    ~ConnectionService();

    swganh::service::ServiceDescription GetServiceDescription();
    
    bool RemoveSession(std::shared_ptr<swganh::network::soe::Session> session);

    std::shared_ptr<swganh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint);
    
    std::shared_ptr<swganh::connection::ConnectionClientInterface> FindConnectionByPlayerId(uint64_t player_id);
    
    void Startup();
    void Shutdown();

    const std::string& listen_address();

    uint16_t listen_port();
        
private:        
    std::shared_ptr<swganh::network::soe::Session> CreateSession(const boost::asio::ip::udp::endpoint& endpoint);
    
    void HandleClientIdMsg_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::ClientIdMsg* message);

    void HandleCmdSceneReady_(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client, 
        swganh::messages::CmdSceneReady* message);
   
    typedef std::map<
        boost::asio::ip::udp::endpoint,
        std::shared_ptr<swganh::connection::ConnectionClientInterface>
    > SessionMap;
    
    boost::mutex session_map_mutex_;
    SessionMap session_map_;

    swganh::app::SwganhKernel* kernel_;
    std::shared_ptr<swganh::connection::PingServer> ping_server_;
    
    std::shared_ptr<swganh::connection::providers::SessionProviderInterface> session_provider_;
    std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;

    swganh::character::CharacterServiceInterface* character_service_;
    swganh::login::LoginServiceInterface* login_service_;
    swganh::simulation::SimulationServiceInterface* simulation_service_;

    swganh::ActiveObject active_;
    std::string listen_address_;
    uint16_t listen_port_;
    uint16_t ping_port_;
    std::shared_ptr<boost::asio::deadline_timer> session_timer_;
};
    
}}  // namespace swganh::connection
