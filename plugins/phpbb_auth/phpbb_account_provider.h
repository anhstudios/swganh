#ifndef PLUGINS_PHPBB_AUTH_PHPBB_ACCOUNT_PROVIDER_H_
#define PLUGINS_PHPBB_AUTH_PHPBB_ACCOUNT_PROVIDER_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/database/database_manager_interface.h"
#include "swganh/login/providers/mysql_account_provider.h"

namespace plugins {
namespace phpbb_auth {

class PhpbbAccountProvider : public swganh::login::providers::MysqlAccountProvider 
{
public:
    PhpbbAccountProvider(anh::database::DatabaseManagerInterface* database_manager,
        std::string table_prefix);

    std::shared_ptr<swganh::login::Account> FindByUsername(std::string username);
    bool AutoRegisterAccount(std::string username, std::string password);

private:
    anh::database::DatabaseManagerInterface* database_manager_;
    std::string table_prefix_;
};

}}  // namespace plugins::phpbb_auth

#endif  // PLUGINS_PHPBB_AUTH_PHPBB_ACCOUNT_PROVIDER_H_