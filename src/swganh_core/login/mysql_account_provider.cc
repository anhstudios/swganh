// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "mysql_account_provider.h"

#include <boost/date_time/posix_time/posix_time.hpp>

#include "swganh/logger.h"

#include "swganh/database/database_manager.h"


using namespace swganh::login;
using namespace providers;
using namespace swganh::login;
using namespace std;

MysqlAccountProvider::MysqlAccountProvider(swganh::database::DatabaseManager* db_manager)
    : AccountProviderInterface()
    , db_manager_(db_manager) {}

shared_ptr<Account> MysqlAccountProvider::FindByUsername(string username) {
    shared_ptr<Account> account = nullptr;

    try {
        auto conn = db_manager_->getConnection("galaxy_manager");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_FindAccountByUsername(?);"));
        statement->setString(1, username);
        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());

        if (result_set->next()) {
            account = make_shared<Account>(true);

            account->account_id(result_set->getInt("id"));
            account->username(result_set->getString("username"));
            account->password(result_set->getString("password"));
            account->salt(result_set->getString("salt"));
            if (result_set->getInt("enabled") == 1) {
                account->Enable();
            } else {
                account->Disable();
            }

            account->algorithm("sha1");
        } else {
            LOG(warning) << "No account information found for user: " << username << endl;
        }
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return account;
}
void MysqlAccountProvider::EndSessions()
{
    try {
        auto conn = db_manager_->getConnection("galaxy_manager");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_DeleteSessions();"));
        /* int rows_updated = */statement->executeUpdate();

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
uint32_t MysqlAccountProvider::FindBySessionKey(const string& session_key) {
    uint32_t account_id = 0;

     try {
        auto conn = db_manager_->getConnection("galaxy_manager");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement("CALL sp_FindAccountBySessionKey(?)"));
        statement->setString(1, session_key);
        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());

        if (result_set->next()) {
            account_id = result_set->getInt("account");

        } else {
            LOG(warning) << "No account found for session_key: " << session_key << endl;
        }
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
     return account_id;
}
bool MysqlAccountProvider::CreateAccountSession(uint32_t account_id, const std::string& session_key) {
    bool success = false;
    try {
        string sql = "call sp_CreateAccountSession(?,?);";
        auto conn = db_manager_->getConnection("galaxy_manager");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, account_id);
        statement->setString(2, session_key);
        auto rows_updated = statement->executeUpdate();
        if (rows_updated > 0)
            success = true;

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return success;
}

bool MysqlAccountProvider::AutoRegisterAccount(std::string username, std::string password) {
    bool success = false;
    try {
        string sql = "call sp_CreateAccount(?,?,?);";
        auto conn = db_manager_->getConnection("galaxy_manager");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setString(1, username);
        statement->setString(2, password);
        statement->setString(3, "");
        auto results = unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (results->next())
		{
			if (CreatePlayerAccount(results->getUInt64(1)))
				success = true;
		}
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return success;
}
bool MysqlAccountProvider::CreatePlayerAccount(uint64_t account_id)
{
	bool success = false;
    try {
        string sql = "call sp_CreatePlayerAccount(?);";
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setUInt64(1, account_id);
        auto rows_updated = statement->executeUpdate();
        if (rows_updated > 0)
            success = true;
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return success;
}