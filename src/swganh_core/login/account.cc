// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh_core/login/account.h"

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
