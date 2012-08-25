// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include "anh/network/soe/session.h"

namespace swganh {
namespace object {
    class ObjectController;
}}  // namespace swganh::object

namespace swganh {
namespace connection {

class ConnectionClientInterface : public anh::network::soe::Session
{
public:
    enum State
    {
        CONNECTING = 0,
        CONNECTED,
        PLAYING,
        DISCONNECTING
    };
   
	ConnectionClientInterface(anh::network::soe::ServerInterface* server, boost::asio::io_service& io_service, 
									boost::asio::ip::udp::endpoint remote_endpoint) 
		: Session(server, io_service, remote_endpoint)
	{
	}

    virtual State GetState() const = 0;

    virtual uint32_t GetAccountId() const = 0;

    virtual uint64_t GetPlayerId() const = 0;

    virtual void Connect(uint32_t account_id, uint64_t player_id) = 0;
    
    virtual const std::shared_ptr<swganh::object::ObjectController>& GetController() const = 0;

    virtual void SetController(const std::shared_ptr<swganh::object::ObjectController>& controller) = 0;

};

}}  // namespace swganh::connection
