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

#ifndef SWGANH_LOGIN_LOGIN_CLIENT_H_
#define SWGANH_LOGIN_LOGIN_CLIENT_H_

#include <memory>
#include <string>

#include "swganh/network/remote_client.h"

namespace anh {
namespace network {
namespace soe {
class Session;
}}}  // namespace anh::network::soe

namespace swganh {
namespace login {

class Account;

class LoginClient : public swganh::network::RemoteClient {
public:    
    explicit LoginClient(
        std::shared_ptr<anh::network::soe::Session> session);
    
    ~LoginClient();

    std::string GetUsername() const;
    void SetUsername(std::string username);

    std::string GetPassword() const;
    void SetPassword(std::string password);

    std::string GetVersion() const;
    void SetVersion(std::string version);

    const std::shared_ptr<Account>& GetAccount() const;
    void SetAccount(const std::shared_ptr<Account>& account);

private:

    LoginClient();

    std::string username_;
    std::string password_;
    std::string version_;
    std::shared_ptr<Account> account_;
};

}}  // namespace swganh::login

#endif  // SWGANH_LOGIN_LOGIN_CLIENT_H_
