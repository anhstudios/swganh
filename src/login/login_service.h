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
#include "swganh/character/character_service_interface.h"

namespace anh {
namespace network {
namespace soe {
class Server;
}}}  // namespace anh::network::soe

namespace anh {
namespace database {
class DatabaseManagerInterface; 
}}  // namespace anh::database

namespace login {
    
class AuthenticationManager;

namespace providers {
class AccountProviderInterface;
}

class LoginClient;

class LoginService : public swganh::base::BaseService {
public:
    explicit LoginService(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher,
                          std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager);
    ~LoginService();

    void DescribeConfigOptions(boost::program_options::options_description& description);

    void onStart();
    void onStop();
    void Update();

    void subscribe();
    
    std::shared_ptr<swganh::character::CharacterServiceInterface> character_service();
    void character_service(std::shared_ptr<swganh::character::CharacterServiceInterface> character_service);

private:
    LoginService();

    bool HandleLoginClientId_(std::shared_ptr<anh::event_dispatcher::EventInterface> incoming_event);
    
    std::unique_ptr<anh::network::soe::Server> soe_server_;
    std::shared_ptr<swganh::character::CharacterServiceInterface> character_service_;
    std::shared_ptr<AuthenticationManager> authentication_manager_;
    std::shared_ptr<providers::AccountProviderInterface> account_provider_;

    typedef std::map<uint32_t, std::shared_ptr<LoginClient>> ClientMap;
    ClientMap clients_;

    uint16_t listen_port_;
};

}  // namespace login
#endif  // LOGIN_LOGIN_SERVICE_H_