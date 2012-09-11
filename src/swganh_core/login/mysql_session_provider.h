// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/connection/providers/session_provider_interface.h"
#include <memory>

namespace swganh { namespace database { class DatabaseManagerInterface; 
}}  // swganh::database

namespace swganh {
namespace login {

class MysqlSessionProvider : public swganh::connection::providers::SessionProviderInterface {
public:
    explicit MysqlSessionProvider(swganh::database::DatabaseManagerInterface* db_manager);
    ~MysqlSessionProvider();

    virtual uint64_t GetPlayerId(uint32_t account_id);
    virtual uint32_t GetAccountId(uint64_t player_id);
    virtual bool CreateGameSession(uint64_t player_id, uint32_t session_id);
	virtual void EndGameSession(uint64_t player_id);
private:
    swganh::database::DatabaseManagerInterface* db_manager_;
};

}}  // namespace swganh::login

