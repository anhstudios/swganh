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

#include "mysql_session_provider.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "anh/logger.h"

#include "anh/database/database_manager.h"


using namespace swganh::connection;
using namespace plugins::mysql_auth;
using namespace std;

MysqlSessionProvider::MysqlSessionProvider(anh::database::DatabaseManagerInterface* db_manager)
    : SessionProviderInterface()
    , db_manager_(db_manager) {}

MysqlSessionProvider::~MysqlSessionProvider() {}

uint64_t MysqlSessionProvider::GetPlayerId(uint32_t account_id) {
    uint64_t player_id = 0;

    try {
        string sql = "select id from player_account where reference_id = ?";
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt(1, account_id);
        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());
        
        if (result_set->next()) {
            player_id = result_set->getUInt64("id");
            
        } else {
            LOG(warning) << "No Player Id found for account_id: " << account_id << endl;
        }

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return player_id;
}

bool MysqlSessionProvider::CreateGameSession(uint64_t player_id, uint32_t session_id) {
    bool updated = false;
    // create new game session 
    std::string game_session = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time())
        + boost::lexical_cast<std::string>(session_id);

    try {
        string sql = "INSERT INTO player_session(player,session_key) VALUES (?,?)";
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, player_id);
        statement->setString(2, game_session);
        auto rows_updated = statement->executeUpdate();
        
        if (rows_updated > 0) {
           updated = true;
            
        } else {
            LOG(warning) << "Couldn't create session for player " << player_id << endl;
        }

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return updated;
}
void MysqlSessionProvider::EndGameSession(uint64_t player_id)
{
	try {
        string sql = "DELETE FROM player_session where player = ?";
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, player_id);
        auto rows_updated = statement->executeUpdate();
        
        if (rows_updated <= 0) {
            LOG(warning) << "Couldn't delete session for player " << player_id << endl;
        }

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
uint32_t MysqlSessionProvider::GetAccountId(uint64_t player_id) {
    uint32_t account_id = 0;

    try {
        string sql = "select reference_id from player_account where id = ?";
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, player_id);
        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());
        
        if (result_set->next()) {
            account_id = result_set->getUInt("reference_id");
            
        } else {
            LOG(warning) << "No account Id found for player id : " << player_id << endl;
        }

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return account_id;
}