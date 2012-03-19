
#include "smf_encoder.h"

#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/sqlstring.h>

#include "anh/logger.h"

#include "anh/database/database_manager_interface.h"

using namespace anh::database;
using namespace plugins::smf_auth;
using namespace std;

SmfEncoder::SmfEncoder(DatabaseManagerInterface* database_manager)
: database_manager_(database_manager)
{
}

string SmfEncoder::EncodePassword(
    string raw, 
    string test)
{
    string result;

    string sql = "SELECT SHA1(CONCAT('" + test + "', '" + raw + "'))";
    auto conn = database_manager_->getConnection("galaxy_manager");
    auto statement = shared_ptr<sql::Statement>(conn->createStatement());
    auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery(sql));
    if (result_set->next())
    {
        result = result_set->getString(1);
    }
    else
        LOG(warning) << "SmfEncoder::EncodePassword failed to encode password" << endl;

    return result;
}

bool SmfEncoder::IsPasswordValid(
    std::string encoded, 
    std::string raw, 
    std::string salt)
{
    return encoded == EncodePassword(move(raw), move(salt));
}
