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

#include "login/providers/mysql_account_provider.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <glog/logging.h>

#include "anh/database/database_manager.h"


using namespace login;
using namespace providers;
using namespace std;

MysqlAccountProvider::MysqlAccountProvider(std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager)
    : AccountProviderInterface()
    , db_manager_(db_manager) {}

MysqlAccountProvider::~MysqlAccountProvider() {}

shared_ptr<Account> MysqlAccountProvider::FindByUsername(string username) {
    shared_ptr<Account> account = nullptr;

    try {
        string sql = "select id, username, password, salt, enabled from account where username = ?";
        auto conn = db_manager_->getConnection("galaxy_manager");
        auto statement = shared_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        statement->setString(1, username);
        auto result_set = statement->executeQuery();
        
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

            account->algorithm("sha512");
        } else {
            DLOG(WARNING) << "No account information found for user: " << username << endl;
        }

    } catch(sql::SQLException &e) {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return account;
}
