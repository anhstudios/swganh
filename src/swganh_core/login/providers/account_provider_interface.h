// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <string>
#include <tuple>

#include "swganh_core/login/account.h"

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
