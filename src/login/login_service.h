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

#ifndef LOGIN_LOGIN_SERVICE_H_
#define LOGIN_LOGIN_SERVICE_H_

#include <map>

#include "anh/network/soe/server.h"

#include "swganh/base/base_service.h"
#include "swganh/character/base_character_service.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

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

namespace login {
    
class AuthenticationManager;

namespace providers {
class AccountProviderInterface;
}

struct LoginClient;

struct GalaxyStatus {    
    uint32_t galaxy_id;
    std::string name;
    std::string address;
    uint16_t connection_port;
    uint16_t ping_port;
    uint32_t server_population;
    uint32_t max_population;
    uint32_t max_characters;
    uint32_t distance;
    uint32_t status;
};

class LoginService : public swganh::base::BaseService {
public:
    explicit LoginService(std::shared_ptr<anh::app::KernelInterface> kernel);
    ~LoginService();
    
    anh::service::Service GetServiceDescription();

    void DescribeConfigOptions(boost::program_options::options_description& description);

    void onStart();
    void onStop();
    void onUpdate();

    void subscribe();
    
    std::shared_ptr<swganh::character::BaseCharacterService> character_service();
    void character_service(std::shared_ptr<swganh::character::BaseCharacterService> character_service);

private:
    LoginService();

    bool HandleLoginClientId_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);
    bool HandleDeleteCharacterMessage_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);

    std::vector<GalaxyStatus> GetGalaxyStatus_();
    
    void SendLoginClientToken_(std::shared_ptr<LoginClient> login_client);
    void SendLoginEnumCluster_(std::shared_ptr<LoginClient> login_client);
    void SendLoginClusterStatus_(std::shared_ptr<LoginClient> login_client);
    void SendEnumerateCharacterId_(std::shared_ptr<LoginClient> login_client);
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    std::shared_ptr<swganh::character::BaseCharacterService> character_service_;
    std::shared_ptr<AuthenticationManager> authentication_manager_;
    std::shared_ptr<providers::AccountProviderInterface> account_provider_;

    std::vector<GalaxyStatus> galaxy_status_;

    typedef std::map<uint32_t, std::shared_ptr<LoginClient>> ClientMap;
    ClientMap clients_;

    std::string listen_address_;
    uint16_t listen_port_;
};

}  // namespace login
#endif  // LOGIN_LOGIN_SERVICE_H_