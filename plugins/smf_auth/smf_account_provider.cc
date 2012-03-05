
#include "smf_account_provider.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <boost/log/trivial.hpp>

using namespace anh::database;
using namespace plugins::smf_auth;
using namespace std;
using namespace swganh::login;

SmfAccountProvider::SmfAccountProvider(
    DatabaseManagerInterface* database_manager,
    string table_prefix)
    : MysqlAccountProvider(database_manager)
    , database_manager_(database_manager)
    , table_prefix_(table_prefix)
{}

shared_ptr<Account> SmfAccountProvider::FindByUsername(string username)
{
    shared_ptr<Account> account = nullptr;

    try 
    {
        string sql = "select id_member, member_name, passwd, id_group from " + table_prefix_ + "members where member_name = ?";
        auto conn = database_manager_->getConnection("smf");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setString(1, username);
        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());
        
        if (result_set->next()) 
        {
            account = make_shared<Account>(true);

            account->account_id(result_set->getInt("id_member"));
            account->username(result_set->getString("member_name"));
            account->password(result_set->getString("passwd"));
            account->salt(result_set->getString("member_name"));

            uint32_t group_id = result_set->getInt("id_group");
            if (group_id != 0) {
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

bool SmfAccountProvider::AutoRegisterAccount(
    std::string username, 
    std::string password)
{
    throw std::runtime_error("Auto registration is not allowed with the PhpbbAuth plugin");
}
