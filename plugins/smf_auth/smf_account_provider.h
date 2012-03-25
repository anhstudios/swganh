#ifndef PLUGINS_SMF_AUTH_SMF_ACCOUNT_PROVIDER_H_
#define PLUGINS_SMF_AUTH_SMF_ACCOUNT_PROVIDER_H_

#include <cstdint>
#include <memory>
#include <string>

#include "anh/database/database_manager_interface.h"
#include "swganh/login/providers/account_provider_interface.h"

namespace plugins {
namespace smf_auth {

class SmfAccountProvider : public swganh::login::providers::AccountProviderInterface
{
public:
    SmfAccountProvider(anh::database::DatabaseManagerInterface* database_manager,
        std::string table_prefix);

    std::shared_ptr<swganh::login::Account> FindByUsername(std::string username);
    bool AutoRegisterAccount(std::string username, std::string password);
    virtual void EndSessions();
    virtual uint32_t FindBySessionKey(const std::string& session_key);
    virtual bool CreateAccountSession(uint32_t account_id, const std::string& session_key);
private:
    anh::database::DatabaseManagerInterface* database_manager_;
    std::string table_prefix_;
};

}}  // namespace plugins::smf_auth

#endif  // PLUGINS_SMF_AUTH_SMF_ACCOUNT_PROVIDER_H_