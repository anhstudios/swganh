// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/connection/providers/session_provider_interface.h"
#include <memory>

namespace swganh { namespace database { class DatabaseManager; 
}}  // swganh::database

namespace swganh {
namespace login {

/**
* Provides sessions via Mysql
*/
class MysqlSessionProvider : public swganh::connection::providers::SessionProviderInterface {
public:
	/**
	* Creates a new instance
	*/
    explicit MysqlSessionProvider(swganh::database::DatabaseManager* db_manager);

	/**
	* Returns the id of a player based on account_id
	*/
    virtual uint64_t GetPlayerId(uint32_t account_id);
    
	/**
	* Returns the id of an account based on player_id
	*/
	virtual uint32_t GetAccountId(uint64_t player_id);

	/**
	* Creates a new game session with the given id
	*/
    virtual bool CreateGameSession(uint64_t player_id, uint32_t session_id);

	/**
	* Ends a game session by player_id
	*/
	virtual void EndGameSession(uint64_t player_id);
private:
    swganh::database::DatabaseManager* db_manager_;
};

}}  // namespace swganh::login

