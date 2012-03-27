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

#include "sha512_encoder.h"

#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/sqlstring.h>

#include "anh/logger.h"

#include "anh/database/database_manager.h"

using namespace anh::database;
using namespace plugins::mysql_auth;
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
