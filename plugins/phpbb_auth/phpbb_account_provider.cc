
#include "phpbb_account_provider.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <boost/log/trivial.hpp>

using namespace anh::database;
using namespace plugins::phpbb_auth;
using namespace std;
using namespace swganh::login;

PhpbbAccountProvider::PhpbbAccountProvider(
    DatabaseManagerInterface* database_manager,
    string table_prefix)
    : MysqlAccountProvider(database_manager)
    , database_manager_(database_manager)
    , table_prefix_(table_prefix)
{}

shared_ptr<Account> PhpbbAccountProvider::FindByUsername(string username)
{
    shared_ptr<Account> account = nullptr;

    try 
    {
        string sql = "select user_id, username, user_password, user_form_salt, group_id from " + table_prefix_ + "users where username = ?";
        auto conn = database_manager_->getConnection("phpbb");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setString(1, username);
        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());
        
        if (result_set->next()) 
        {
            account = make_shared<Account>(true);

            account->account_id(result_set->getInt("user_id"));
            account->username(result_set->getString("username"));
            account->password(result_set->getString("user_password"));
            account->salt(result_set->getString("user_form_salt"));

            uint32_t group_id = result_set->getInt("group_id");
            if (group_id == 5 || group_id == 2) {
                account->Enable();
            } else {
                account->Disable();
            }
        } 
        else 
        {
            BOOST_LOG_TRIVIAL(warning) << "No account information found for user: " << username << endl;
        }

    } 
    catch(sql::SQLException &e) 
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return account;
}

bool PhpbbAccountProvider::AutoRegisterAccount(
    std::string username, 
    std::string password)
{
    throw std::runtime_error("Auto registration is not allowed with the PhpbbAuth plugin");
}
