// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_LOGIN_LOGIN_SERVICE_H_
#define SWGANH_LOGIN_LOGIN_SERVICE_H_

#include <unordered_map>

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>

#include "anh/active_object.h"
#include "anh/logger.h"

#include "anh/network/soe/packet_utilities.h"
#include "anh/network/soe/server.h"
#include "anh/service/service_interface.h"

#include "swganh/network/base_swg_server.h"

#include "swganh/character/character_service.h"
#include "swganh/galaxy/galaxy_service.h"

#include "swganh/login/login_client.h"

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
    
class AuthenticationManager;

namespace providers {
class AccountProviderInterface;
}

class LoginService 
    : public anh::service::ServiceInterface
    , public swganh::network::BaseSwgServer
{
public:
    LoginService(
        std::string listen_address, 
        uint16_t listen_port, 
        swganh::app::SwganhKernel* kernel);
    ~LoginService();
    
    anh::service::ServiceDescription GetServiceDescription();
    
    bool RemoveSession(std::shared_ptr<anh::network::soe::Session> session);

    std::shared_ptr<anh::network::soe::Session> GetSession(const boost::asio::ip::udp::endpoint& endpoint);

    uint32_t GetAccountBySessionKey(const std::string& session_key);
        
    int galaxy_status_check_duration_secs() const;
    void galaxy_status_check_duration_secs(int new_duration);

    bool login_auto_registration() const;
    void login_auto_registration(bool auto_registeration);
    
    int login_error_timeout_secs() const;
    void login_error_timeout_secs(int new_timeout);
    
    void Startup();

    void Shutdown();

private:
    LoginService();
    
    std::shared_ptr<anh::network::soe::Session> CreateSession(const boost::asio::ip::udp::endpoint& endpoint);
        
    void HandleLoginClientId_(const std::shared_ptr<LoginClient>& login_client, swganh::messages::LoginClientId message);

    std::vector<GalaxyStatus> GetGalaxyStatus_();
    void UpdateGalaxyStatus_();
    
    typedef std::map<
        boost::asio::ip::udp::endpoint,
        std::shared_ptr<LoginClient>
    > SessionMap;
    
    boost::mutex session_map_mutex_;
    SessionMap session_map_;

    swganh::app::SwganhKernel* kernel_;
    swganh::character::CharacterService* character_service_;
    std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;

	swganh::galaxy::GalaxyService* galaxy_service_;
    std::shared_ptr<AuthenticationManager> authentication_manager_;
    std::shared_ptr<providers::AccountProviderInterface> account_provider_;
    
    std::vector<GalaxyStatus> galaxy_status_;
    
    bool login_auto_registration_;
    int galaxy_status_check_duration_secs_;
    int login_error_timeout_secs_;
    boost::asio::deadline_timer galaxy_status_timer_;
    std::shared_ptr<boost::asio::deadline_timer> session_timer_;
    
    std::string listen_address_;
    uint16_t listen_port_;
    anh::ActiveObject active_;
};

}} // namespace swganh::login

#endif  // SWGANH_LOGIN_LOGIN_SERVICE_H_
