// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef MYSQL_AUTH_MYSQL_SESSION_PROVIDER_H_
#define MYSQL_AUTH_MYSQL_SESSION_PROVIDER_H_

#include "swganh/connection/providers/session_provider_interface.h"
#include <memory>

namespace anh { namespace database { class DatabaseManagerInterface; 
}}  // anh::database

namespace swganh_core {
namespace login {

class MysqlSessionProvider : public swganh::connection::providers::SessionProviderInterface {
public:
    explicit MysqlSessionProvider(anh::database::DatabaseManagerInterface* db_manager);
    ~MysqlSessionProvider();

    virtual uint64_t GetPlayerId(uint32_t account_id);
    virtual uint32_t GetAccountId(uint64_t player_id);
    virtual bool CreateGameSession(uint64_t player_id, uint32_t session_id);
	virtual void EndGameSession(uint64_t player_id);
private:
    anh::database::DatabaseManagerInterface* db_manager_;
};

}}  // namespace swganh::login

#endif  // MYSQL_AUTH_MYSQL_SESSION_PROVIDER_H_
