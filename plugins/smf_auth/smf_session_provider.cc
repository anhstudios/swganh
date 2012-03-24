
#include "smf_session_provider.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "anh/logger.h"

using namespace anh::database;
using namespace plugins::smf_auth;
using namespace std;
using namespace swganh::connection;


SmfSessionProvider::SmfSessionProvider(
    DatabaseManagerInterface* database_manager,
    string table_prefix)
    : database_manager_(database_manager)
    , table_prefix_(table_prefix)
{}

uint64_t SmfSessionProvider::GetPlayerId(uint32_t account_id)
{
    uint64_t player_id = FindPlayerByReferenceId_(account_id);

    if (player_id == 0)
    {
        CreatePlayerAccount_(account_id);
        player_id = FindPlayerByReferenceId_(account_id);
    }

    return player_id;
}

bool SmfSessionProvider::CreatePlayerAccount_(uint64_t account_id)
{
	bool success = false;
    try 
    {
        string sql = "call sp_CreatePlayerAccount(?);";
        auto conn = database_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, account_id);
        auto rows_updated = statement->executeUpdate();
        if (rows_updated > 0)
            success = true;
    } 
    catch(sql::SQLException &e) 
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return success;
}
    
uint64_t SmfSessionProvider::FindPlayerByReferenceId_(uint64_t account_id)
{
    uint64_t player_id = 0;

    try {
        string sql = "select id from player_account where reference_id = ?";
        auto conn = database_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, account_id);
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

bool SmfSessionProvider::CreateGameSession(uint64_t player_id, uint32_t session_id) {
    bool updated = false;
    // create new game session 
    std::string game_session = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time())
        + boost::lexical_cast<std::string>(session_id);

    try {
        string sql = "INSERT INTO player_session(player,session_key) VALUES (?,?)";
        auto conn = database_manager_->getConnection("galaxy");
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
void SmfSessionProvider::EndGameSession(uint64_t player_id)
{
	try {
        string sql = "DELETE FROM player_session where player = ?";
        auto conn = database_manager_->getConnection("galaxy");
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
uint32_t SmfSessionProvider::GetAccountId(uint64_t player_id) {
    uint32_t account_id = 0;

    try {
        string sql = "select reference_id from player_account where id = ?";
        auto conn = database_manager_->getConnection("galaxy");
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