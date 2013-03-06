// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include "swganh/network/soe/session.h"

namespace swganh {
namespace observer {
	class ObserverInterface;
}
}

namespace swganh {
namespace connection {

class ConnectionClientInterface : public swganh::network::soe::Session
{
public:
    enum State
    {
        CONNECTING = 0,
        CONNECTED,
        PLAYING,
        DISCONNECTING
    };
   
	ConnectionClientInterface(swganh::network::soe::ServerInterface* server, boost::asio::io_service& cpu_pool, 
									boost::asio::ip::udp::endpoint remote_endpoint) 
		: Session(server, cpu_pool, remote_endpoint)
	{
	}

    virtual State GetState() const = 0;

    virtual uint32_t GetAccountId() const = 0;

    virtual uint64_t GetPlayerId() const = 0;

    virtual void Connect(uint32_t account_id, uint64_t player_id) = 0;
    
    virtual const std::shared_ptr<swganh::observer::ObserverInterface>& GetController() const = 0;

    virtual void SetController(const std::shared_ptr<swganh::observer::ObserverInterface>& controller) = 0;

};

}}  // namespace swganh::connection
