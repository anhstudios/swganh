
#ifndef PLUGINS_SMF_AUTH_SMF_SESSION_PROVIDER_H_
#define PLUGINS_SMF_AUTH_SMF_SESSION_PROVIDER_H_

#include <cstdint>

#include "anh/database/database_manager_interface.h"
#include "swganh/connection/providers/mysql_session_provider.h"

namespace plugins {
namespace smf_auth {

class SmfSessionProvider : public swganh::connection::providers::MysqlSessionProvider 
{
public:
    SmfSessionProvider(anh::database::DatabaseManagerInterface* database_manager,
        std::string table_prefix);

    uint64_t GetPlayerId(uint32_t account_id);

private:
    bool CreatePlayerAccount_(uint64_t account_id);
    uint64_t FindPlayerByReferenceId_(uint64_t account_id);

    anh::database::DatabaseManagerInterface* database_manager_;
    std::string table_prefix_;
};

}}  // namespace plugins::smf_auth

#endif  // PLUGINS_SMF_AUTH_SMF_SESSION_PROVIDER_H_
