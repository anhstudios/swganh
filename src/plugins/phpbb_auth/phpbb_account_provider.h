#ifndef PLUGINS_PHPBB_AUTH_PHPBB_ACCOUNT_PROVIDER_H_
#define PLUGINS_PHPBB_AUTH_PHPBB_ACCOUNT_PROVIDER_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/database/database_manager_interface.h"
#include "swganh/login/providers/account_provider_interface.h"

namespace plugins {
namespace phpbb_auth {

class PhpbbAccountProvider : public swganh::login::providers::AccountProviderInterface 
{
public:
    PhpbbAccountProvider(const std::shared_ptr<anh::database::DatabaseManagerInterface>& database_manager);
    ~PhpbbAccountProvider() {}

    std::shared_ptr<swganh::login::Account> FindByUsername(std::string username);
    uint32_t FindBySessionKey(const std::string& session_key);
    bool CreateAccountSession(uint32_t account_id, const std::string& session_key);
    bool AutoRegisterAccount(std::string username, std::string password);

private:
    bool CreatePlayerAccount_(uint64_t account_id);

    std::shared_ptr<anh::database::DatabaseManagerInterface> database_manager_;
};

}}  // namespace plugins::phpbb_auth

#endif  // PLUGINS_PHPBB_AUTH_PHPBB_ACCOUNT_PROVIDER_H_