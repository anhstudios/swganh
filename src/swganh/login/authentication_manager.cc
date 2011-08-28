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

#include "swganh/login/authentication_manager.h"

#include "swganh/login/account.h"
#include "swganh/login/login_client.h"

using namespace swganh::login;
using namespace std;

AuthenticationManager::AuthenticationManager(std::shared_ptr<encoders::EncoderInterface> encoder) 
    : encoder_(encoder) {}

std::shared_ptr<encoders::EncoderInterface> AuthenticationManager::encoder() {
    return encoder_;
}

bool AuthenticationManager::Authenticate(std::shared_ptr<LoginClient> client, std::shared_ptr<Account> account) {
    auto current_account = client->account;

    if (current_account) {
        // make sure the password hasn't changed
        if (current_account->password() != account->password()) {
            // consider throwing here perhaps?
            return false;
        }
    } else {
        string presented_password = client->password;
        if (presented_password.empty()) {
            return false;
        }

        if (!encoder_->IsPasswordValid(account->password(), presented_password, account->salt())) {
            return false;
        }
    }

    return true;
}
