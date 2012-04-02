/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2012 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_LOGIN_LOGIN_SERVICE_H_
#define SWGANH_LOGIN_LOGIN_SERVICE_H_

#include <mutex>
#include <unordered_map>

#include <boost/asio.hpp>

#include "anh/logger.h"

#include "anh/network/soe/packet_utilities.h"
#include "anh/network/soe/server.h"

#include "swganh/base/base_service.h"
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
    : public swganh::base::BaseService
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

private:
    LoginService();
    
    std::shared_ptr<anh::network::soe::Session> CreateSession(const boost::asio::ip::udp::endpoint& endpoint);

    void onStart();
    void onStop();

    void subscribe();
    
    void HandleLoginClientId_(const std::shared_ptr<LoginClient>& login_client, swganh::messages::LoginClientId message);

    std::vector<GalaxyStatus> GetGalaxyStatus_();
    void UpdateGalaxyStatus_();
    
    typedef std::map<
        boost::asio::ip::udp::endpoint,
        std::shared_ptr<LoginClient>
    > SessionMap;
    
    std::mutex session_map_mutex_;
    SessionMap session_map_;

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
    
    std::string listen_address_;
    uint16_t listen_port_;
};

}} // namespace swganh::login

#endif  // SWGANH_LOGIN_LOGIN_SERVICE_H_
