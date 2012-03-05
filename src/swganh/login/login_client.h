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

#include "anh/network/soe/session.h"

namespace swganh {
namespace login {

class Account;

class LoginClient : public anh::network::soe::Session {
public:    
    LoginClient(anh::network::soe::ServerInterface* server, boost::asio::io_service& io_service, boost::asio::ip::udp::endpoint remote_endpoint);
    
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
