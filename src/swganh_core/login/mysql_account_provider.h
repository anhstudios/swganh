// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/login/providers/account_provider_interface.h"

namespace swganh { namespace database { class DatabaseManagerInterface; 
}}  // swganh::database

namespace swganh {
namespace login {

class MysqlAccountProvider : public swganh::login::providers::AccountProviderInterface {
public:
    explicit MysqlAccountProvider(swganh::database::DatabaseManagerInterface* db_manager);
    ~MysqlAccountProvider();

    virtual std::shared_ptr<swganh::login::Account> FindByUsername(std::string username);
    virtual uint32_t FindBySessionKey(const std::string& session_key);
    virtual void EndSessions();
    virtual bool CreateAccountSession(uint32_t account_id, const std::string& session_key);
    virtual bool AutoRegisterAccount(std::string username, std::string password);
	virtual bool CreatePlayerAccount(uint64_t account_id);
	
private:
    swganh::database::DatabaseManagerInterface* db_manager_;
};

}}  // namespace swganh::login
