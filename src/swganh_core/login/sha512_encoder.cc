// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "sha512_encoder.h"

#include "swganh/logger.h"

#include "swganh/database/database_manager.h"

using namespace swganh::database;
using namespace swganh::login;
using namespace std;

Sha512Encoder::Sha512Encoder(DatabaseManager* db_manager)
    : db_manager_(db_manager) {}

string Sha512Encoder::EncodePassword(string raw, string salt) {
    string result;

    string sql = "CALL sp_EncodePassword(?,?)";
    auto conn = db_manager_->getConnection("galaxy_manager");
    auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
	statement->setString(1, raw);
	statement->setString(2, salt);
    auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery());
    if (result_set->next())
    {
        result = result_set->getString(1);
    }
    else
        LOG(warning) << "Sha512Encoder::EncodePassword failed to encode password" << endl;
	while(statement->getMoreResults());
    return result;
}

bool Sha512Encoder::IsPasswordValid(string encoded, string raw, string salt) {
    return encoded == EncodePassword(move(raw), move(salt));
}
