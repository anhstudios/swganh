// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef MYSQL_AUTH_ACCOUNT_PROVIDER_H_
#define MYSQL_AUTH_ACCOUNT_PROVIDER_H_

#include "swganh/login/providers/account_provider_interface.h"

namespace anh { namespace database { class DatabaseManagerInterface; 
}}  // anh::database

namespace swganh_core {
namespace login {

class MysqlAccountProvider : public swganh::login::providers::AccountProviderInterface {
public:
    explicit MysqlAccountProvider(anh::database::DatabaseManagerInterface* db_manager);
    ~MysqlAccountProvider();

    virtual std::shared_ptr<swganh::login::Account> FindByUsername(std::string username);
    virtual uint32_t FindBySessionKey(const std::string& session_key);
    virtual void EndSessions();
    virtual bool CreateAccountSession(uint32_t account_id, const std::string& session_key);
    virtual bool AutoRegisterAccount(std::string username, std::string password);
	virtual bool CreatePlayerAccount(uint64_t account_id);
	
private:
    anh::database::DatabaseManagerInterface* db_manager_;
};

}}  // namespace swganh::login

#endif  // MYSQL_AUTH_ACCOUNT_PROVIDER_H_