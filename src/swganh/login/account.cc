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

#include "swganh/login/account.h"

using namespace swganh::login;
using namespace std;

Account::Account(bool enabled) 
    : enabled_(enabled) {}

Account::~Account() {}

uint32_t Account::account_id() const {
    return account_id_;
}

void Account::account_id(uint32_t account_id) {
    account_id_ = account_id;
}

string Account::username() const {
    return username_;
}

void Account::username(string username) {
    username_ = move(username);
}

string Account::salt() const {
    return salt_;
}

void Account::salt(string salt) {
    salt_ = move(salt);
}

string Account::password() const {
    return password_;
}

void Account::password(string password) {
    password_ = move(password);
}

string Account::algorithm() const {
    return algorithm_;
}

void Account::algorithm(string algorithm) {
    algorithm_ = move(algorithm);
}

bool Account::IsEnabled() const {
    return enabled_;
}

void Account::Enable() {
    enabled_ = true;
}

void Account::Disable() {
    enabled_ = false;
}
