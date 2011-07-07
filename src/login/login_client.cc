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

#include "login/login_client.h"

#include "login/account.h"

using namespace login;
using namespace std;

string LoginClient::username() const {
    return username_;
}

void LoginClient::username(string username) {
    username_ = username;
}

string LoginClient::password() const {
    return password_;
}

void LoginClient::password(string password) {
    password_ = password;
}

string LoginClient::version() const {
    return version_;
}

void LoginClient::version(string version) {
    version_ = version;
}

shared_ptr<Account> LoginClient::account() const {
    return account_;
}

void LoginClient::account(shared_ptr<Account> account) {
    account_ = account;
}
