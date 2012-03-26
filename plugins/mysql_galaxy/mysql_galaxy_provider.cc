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

#include "mysql_galaxy_provider.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "anh/logger.h"

#include "anh/database/database_manager.h"

using namespace plugins::galaxy;
using namespace std;

MysqlGalaxyProvider::MysqlGalaxyProvider(anh::database::DatabaseManagerInterface* db_manager)
    : GalaxyProviderInterface()
    , db_manager_(db_manager) {}

MysqlGalaxyProvider::~MysqlGalaxyProvider() {}

uint32_t MysqlGalaxyProvider::GetPopulation()
{
	uint32_t population = 0;
    try {
        string sql = "call sp_GetPopulation();";
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        auto result_set = unique_ptr<sql::ResultSet>(statement->executeQuery(sql));

		if (result_set->next())
			population = result_set->getUInt(1);

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return population;
}