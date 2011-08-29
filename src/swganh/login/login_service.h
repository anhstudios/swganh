/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

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

#include <boost/asio.hpp>
#include <glog/logging.h>
#include <tbb/concurrent_hash_map.h>

#include "anh/network/soe/packet_utilities.h"
#include "anh/network/soe/server.h"

#include "swganh/base/base_service.h"
#include "swganh/base/swg_message_router.h"

#include "swganh/character/character_service.h"

#include "swganh/login/login_client.h"

#include "swganh/login/galaxy_status.h"
#include "swganh/login/messages/login_client_id.h"

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
namespace login {
    
class AuthenticationManager;

namespace providers {
class AccountProviderInterface;
}

class LoginService 
    : public swganh::base::BaseService 
    , public swganh::base::SwgMessageRouter<LoginClient>
{
public:
    typedef tbb::concurrent_hash_map<
        boost::asio::ip::udp::endpoint, 
        std::shared_ptr<swganh::login::LoginClient>,
        anh::network::soe::EndpointHashCompare
    > ClientMap;

    LoginService(
        std::string listen_address, 
        uint16_t listen_port, 
        std::shared_ptr<anh::app::KernelInterface> kernel);
    ~LoginService();
    
    anh::service::ServiceDescription GetServiceDescription();

    uint32_t GetAccountBySessionKey(const std::string& session_key);
        
    std::shared_ptr<swganh::login::LoginClient> GetClientFromEndpoint(
        const boost::asio::ip::udp::endpoint& remote_endpoint);

    int galaxy_status_check_duration_secs() const;
    void galaxy_status_check_duration_secs(int new_duration);
    
    int login_error_timeout_secs() const;
    void login_error_timeout_secs(int new_timeout);

private:
    LoginService();

    void onStart();
    void onStop();

    void subscribe();
    
    void HandleLoginClientId_(std::shared_ptr<swganh::login::LoginClient> login_client, const messages::LoginClientId& message);

    void RemoveClient_(std::shared_ptr<anh::network::soe::Session> session);
    std::shared_ptr<swganh::login::LoginClient> AddClient_(std::shared_ptr<anh::network::soe::Session> session);

    std::vector<GalaxyStatus> GetGalaxyStatus_();
    void UpdateGalaxyStatus_();
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    std::shared_ptr<swganh::character::CharacterService> character_service_;
    std::shared_ptr<AuthenticationManager> authentication_manager_;
    std::shared_ptr<providers::AccountProviderInterface> account_provider_;
    
    std::vector<GalaxyStatus> galaxy_status_;
    
    int galaxy_status_check_duration_secs_;
    int login_error_timeout_secs_;
    boost::asio::deadline_timer galaxy_status_timer_;

    ClientMap clients_;

    std::string listen_address_;
    uint16_t listen_port_;
};

}} // namespace swganh::login

#endif  // SWGANH_LOGIN_LOGIN_SERVICE_H_
