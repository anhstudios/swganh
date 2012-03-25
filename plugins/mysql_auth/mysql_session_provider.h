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

#ifndef MYSQL_AUTH_MYSQL_SESSION_PROVIDER_H_
#define MYSQL_AUTH_MYSQL_SESSION_PROVIDER_H_

#include "swganh/connection/providers/session_provider_interface.h"
#include <memory>

namespace anh { namespace database { class DatabaseManagerInterface; 
}}  // anh::database

namespace plugins {
namespace mysql_auth {

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

}}  // namespace plugins::mysql_auth

#endif  // MYSQL_AUTH_MYSQL_SESSION_PROVIDER_H_
