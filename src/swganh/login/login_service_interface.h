// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_LOGIN_LOGIN_SERVICE_INTERFACE_H_
#define SWGANH_LOGIN_LOGIN_SERVICE_INTERFACE_H_

#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

#include "anh/active_object.h"
#include "anh/logger.h"

#include "anh/network/soe/packet_utilities.h"
#include "anh/network/soe/server.h"
#include "anh/service/service_interface.h"

#include "swganh/network/base_swg_server.h"

#include "swganh/character/character_service_interface.h"
#include "swganh/galaxy/galaxy_service.h"

#include "swganh/login/galaxy_status.h"
#include "swganh/messages/login_client_id.h"

namespace anh {
namespace network {
namespace soe {
class Server;
}}}  // namespace anh::network::soe

namespace anh {
namespace database {
class DatabaseManagerInterface; 
}}  // namespace anh::database

namespace anh {
namespace event_dispatcher {
class EventInterface;
}}  // namespace anh::event_dispatcher

namespace swganh {
namespace character {
class CharacterProviderInterface;
}}

namespace swganh {
namespace login {
    
class AuthenticationManagerInterface;

namespace providers {
class AccountProviderInterface;
}

class LoginServiceInterface
    : public anh::service::ServiceInterface
    , public swganh::network::BaseSwgServer
{
public:
    LoginServiceInterface(boost::asio::io_service& io_service)
		: swganh::network::BaseSwgServer(io_service)
	{
	}
    
    virtual anh::service::ServiceDescription GetServiceDescription() = 0;
    
    virtual bool RemoveSession(std::shared_ptr<anh::network::soe::Session> session) = 0;

    virtual std::shared_ptr<anh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint) = 0;

    virtual uint32_t GetAccountBySessionKey(const std::string& session_key) = 0;
        
    virtual int galaxy_status_check_duration_secs() const = 0;
    virtual void galaxy_status_check_duration_secs(int new_duration) = 0;

    virtual bool login_auto_registration() const = 0;
    virtual void login_auto_registration(bool auto_registeration) = 0;
    
    virtual int login_error_timeout_secs() const = 0;
    virtual void login_error_timeout_secs(int new_timeout) = 0;
    
    virtual void Startup() = 0;
    virtual void Shutdown() = 0;
};

}} // namespace swganh::login

#endif  // SWGANH_LOGIN_LOGIN_SERVICE_H_
