/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef MYSQL_AUTH_ACCOUNT_PROVIDER_H_
#define MYSQL_AUTH_ACCOUNT_PROVIDER_H_

#include "swganh/login/providers/account_provider_interface.h"

namespace anh { namespace database { class DatabaseManagerInterface; 
}}  // anh::database

namespace plugins {
namespace mysql_auth {

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

}}  // namespace plugins::mysql_auth

#endif  // MYSQL_AUTH_ACCOUNT_PROVIDER_H_