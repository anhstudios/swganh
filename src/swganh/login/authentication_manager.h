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

#ifndef SWGANH_LOGIN_AUTHENTICATION_MANAGER_H_
#define SWGANH_LOGIN_AUTHENTICATION_MANAGER_H_

#include <memory>

#include "swganh/login/encoders/encoder_interface.h"

namespace swganh {
namespace login {

class Account;
struct LoginClient;

class AuthenticationManager {
public:
    explicit AuthenticationManager(std::shared_ptr<encoders::EncoderInterface> encoder);

    std::shared_ptr<encoders::EncoderInterface> encoder();

    bool Authenticate(
        std::shared_ptr<swganh::login::LoginClient> client, 
        std::shared_ptr<swganh::login::Account> account);

private:
    std::shared_ptr<encoders::EncoderInterface> encoder_;
};

}}  // namespace swganh::login

#endif  // SWGANH_LOGIN_AUTHENTICATION_MANAGER_H_
