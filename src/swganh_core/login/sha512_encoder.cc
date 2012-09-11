// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "sha512_encoder.h"

#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/sqlstring.h>

#include "swganh/logger.h"

#include "swganh/database/database_manager.h"

using namespace swganh::database;
using namespace swganh::login;
using namespace std;

Sha512Encoder::Sha512Encoder(DatabaseManagerInterface* db_manager)
    : db_manager_(db_manager) {}
Sha512Encoder::~Sha512Encoder() {}

string Sha512Encoder::EncodePassword(string raw, string salt) {
    string result;

    string sql = "SELECT SHA1(CONCAT('" + raw + "', '{" + salt + "}'))";
    auto conn = db_manager_->getConnection("galaxy_manager");
    auto statement = shared_ptr<sql::Statement>(conn->createStatement());
    auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery(sql));
    if (result_set->next())
    {
        result = result_set->getString(1);
    }
    else
        LOG(warning) << "Sha512Encoder::EncodePassword failed to encode password" << endl;

    return result;
}

bool Sha512Encoder::IsPasswordValid(string encoded, string raw, string salt) {
    return encoded == EncodePassword(move(raw), move(salt));
}
