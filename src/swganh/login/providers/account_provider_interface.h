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

#ifndef SWGANH_LOGIN_PROVIDERS_ACCOUNT_PROVIDER_INTERFACE_H_
#define SWGANH_LOGIN_PROVIDERS_ACCOUNT_PROVIDER_INTERFACE_H_

#include <memory>
#include <string>
#include <tuple>

#include "swganh/login/account.h"

namespace swganh {
namespace login {
namespace providers {

class AccountProviderInterface {
public:
    virtual ~AccountProviderInterface() {}

    virtual std::shared_ptr<swganh::login::Account> FindByUsername(std::string username) = 0;
    virtual uint32_t FindBySessionKey(const std::string& session_key) = 0;
    virtual void EndSessions() = 0;
    virtual bool CreateAccountSession(uint32_t account_id, const std::string& session_key) = 0;
    virtual bool AutoRegisterAccount(std::string username, std::string password) = 0;
};

}}}  // namespace swganh::login::providers

#endif  // SWGANH_LOGIN_PROVIDERS_ACCOUNT_PROVIDER_INTERFACE_H_
