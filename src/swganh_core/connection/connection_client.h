// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include "swganh_core/connection/connection_client_interface.h"

namespace swganh {
namespace connection {

/*
* Represents a remote client.
*/
class ConnectionClient : public swganh::connection::ConnectionClientInterface
{
public:

	/**
	* Creates a new instance
	*/
    ConnectionClient(swganh::network::ServerInterface* server, boost::asio::io_service& cpu_pool, boost::asio::ip::udp::endpoint remote_endpoint);
    
	/**
	* @return the current state of this remote client
	*/
    State GetState() const;

	/**
	* @return the account id of this remote client
	*/
    uint32_t GetAccountId() const;

	/**
	* @return the player id of this remote client 
	*/
    uint64_t GetPlayerId() const;

	/**
	* Saves the account and player data and sets the state of this client
	* @param account_id the account to set
	* @param player_id the player id to set
	*/
    void Connect(uint32_t account_id, uint64_t player_id);
    
	/**
	* @return the observer (ie. session) associated with this connection client.
	*/
    const std::shared_ptr<swganh::observer::ObserverInterface>& GetController() const;

	/**
	* @return the observer (ie. session) associated with this connection client
	*/
    void SetController(const std::shared_ptr<swganh::observer::ObserverInterface>& controller);

private:

    void OnClose();

    State state_;
    uint32_t account_id_;
    uint64_t player_id_;
    std::shared_ptr<swganh::observer::ObserverInterface> controller_;
};

}}  // namespace swganh::connection
