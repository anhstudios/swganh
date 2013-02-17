// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/thread/mutex.hpp>

#include "swganh_core/connection/connection_service_interface.h"

namespace swganh {
namespace connection {

class PingServer;

/**
* Manages remote connections
*/
class ConnectionService : public swganh::connection::ConnectionServiceInterface
{
public:
	/**
	* Create a new instance
	*/
    ConnectionService(
        std::string listen_address, 
        uint16_t listen_port, 
        uint16_t ping_port, 
        swganh::app::SwganhKernel* kernel);
    
	/**
	* Custom destructor
	*/
    ~ConnectionService();

	/**
	* @return the service description for this service
	*/
    swganh::service::ServiceDescription GetServiceDescription();
    
	/**
	* Removes a session (ie. ends)
	*/
    bool RemoveSession(std::shared_ptr<swganh::network::soe::Session> session);

	/**
	* Returns the session by endpoint (ie. ip address)
	* @param endpoint the ip address to find
	* @return the session associated with the ip or nullptr
	*/
    std::shared_ptr<swganh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint);
    
	/**
	* Finds a connection client based on player id
	* @param player_id the id of the player to find the connection for
	*/
    std::shared_ptr<swganh::connection::ConnectionClientInterface> FindConnectionByPlayerId(uint64_t player_id);
    
	/**
	* Called on startup
	*/
    void Startup();

	/**
	* Called on shutdown
	*/
    void Shutdown();

	/**
	* @return the string name of the address this connection service is using
	*/
    const std::string& listen_address();

	/**
	* @return the port this connection service is using
	*/
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
