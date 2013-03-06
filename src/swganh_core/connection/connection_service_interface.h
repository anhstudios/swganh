// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/thread/mutex.hpp>

#include "swganh/active_object.h"
#include "swganh/hash_string.h"

#include "swganh/network/soe/packet_utilities.h"
#include "swganh/network/soe/session.h"
#include "swganh/service/service_interface.h"

#include "swganh/network/base_swg_server.h"

#include "swganh_core/login/login_service_interface.h"
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/messages/cmd_scene_ready.h"

#include "swganh_core/connection/providers/session_provider_interface.h"

#include "swganh_core/messages/client_permissions_message.h"
#include "swganh_core/messages/client_id_msg.h"
#include "swganh_core/messages/heart_beat.h"

namespace swganh {
namespace network {
namespace soe {
class Server;
}}}  // namespace swganh::network::soe

namespace swganh {
namespace character {
class CharacterProviderInterface;
class CharacterServiceInterface;
}}

namespace swganh {
namespace connection {
    
class ConnectionClientInterface;

class ConnectionServiceInterface : public swganh::service::ServiceInterface, public swganh::network::BaseSwgServer
{
public:
    
	ConnectionServiceInterface(swganh::app::SwganhKernel* kernel)
		: swganh::network::BaseSwgServer(kernel->GetIoThreadPool())
	{
	}

    virtual bool RemoveSession(std::shared_ptr<swganh::network::soe::Session> session) = 0;

    virtual std::shared_ptr<swganh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint) = 0;
    
    virtual std::shared_ptr<ConnectionClientInterface> FindConnectionByPlayerId(uint64_t player_id) = 0;
    
    virtual void Startup() = 0;
    virtual void Shutdown() = 0;

    virtual const std::string& listen_address() = 0;

    virtual uint16_t listen_port() = 0;
};
    
}}  // namespace swganh::connection
