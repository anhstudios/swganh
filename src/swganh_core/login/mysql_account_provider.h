// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/login/providers/account_provider_interface.h"

namespace swganh { namespace database { class DatabaseManager; 
}}  // swganh::database

namespace swganh {
namespace login {

/**
* Provides account handling via mysql
*/
class MysqlAccountProvider : public swganh::login::providers::AccountProviderInterface {
public:

	/**
	* Create new instance
	*/
    explicit MysqlAccountProvider(swganh::database::DatabaseManager* db_manager);

	/**
	* Finds and populates an account by username
	*/
    virtual std::shared_ptr<swganh::login::Account> FindByUsername(std::string username);
 
	/**
	* Finds an populates an account by session key
	*/
	virtual uint32_t FindBySessionKey(const std::string& session_key);
    
	/**
	* Ends all active sessions 
	*/
	virtual void EndSessions();
	
	/**
	* Creates a new session for an account with the given key
	*/
    virtual bool CreateAccountSession(uint32_t account_id, const std::string& session_key);
    
	/**
	* Creates a new account with the given username and password
	*/
	virtual bool AutoRegisterAccount(std::string username, std::string password);
	
	/**
	* Creates a new player account for the given id
	*/
	virtual bool CreatePlayerAccount(uint64_t account_id);
	
private:
    swganh::database::DatabaseManager* db_manager_;
};

}}  // namespace swganh::login
