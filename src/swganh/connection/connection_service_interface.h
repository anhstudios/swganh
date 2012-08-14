// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CONNECTION_CONNECTION_SERVICE_INTERFACE_H_
#define SWGANH_CONNECTION_CONNECTION_SERVICE_INTERFACE_H_

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

#include "swganh/login/login_service_interface.h"
#include "swganh/simulation/simulation_service_interface.h"
#include "pub14_core/messages/cmd_scene_ready.h"

#include "swganh/connection/providers/session_provider_interface.h"

#include "pub14_core/messages/client_permissions_message.h"
#include "pub14_core/messages/client_id_msg.h"
#include "pub14_core/messages/heart_beat.h"

namespace anh {
namespace network {
namespace soe {
class Server;
}}}  // namespace anh::network::soe

namespace swganh {
namespace character {
class CharacterProviderInterface;
class CharacterServiceInterface;
}}

namespace swganh {
namespace connection {
    
class ConnectionClientInterface;

class ConnectionServiceInterface : public anh::service::ServiceInterface, public swganh::network::BaseSwgServer
{
public:
    
	ConnectionServiceInterface(swganh::app::SwganhKernel* kernel)
		: swganh::network::BaseSwgServer(kernel->GetIoService())
	{
	}

    virtual bool RemoveSession(std::shared_ptr<anh::network::soe::Session> session) = 0;

    virtual std::shared_ptr<anh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint) = 0;
    
    virtual std::shared_ptr<ConnectionClientInterface> FindConnectionByPlayerId(uint64_t player_id) = 0;
    
    virtual void Startup() = 0;
    virtual void Shutdown() = 0;

    virtual const std::string& listen_address() = 0;

    virtual uint16_t listen_port() = 0;
};
    
}}  // namespace swganh::connection

#endif  // SWGANH_CONNECTION_CONNECTION_SERVICE_H_
