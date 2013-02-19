// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "mysql_galaxy_provider.h"

#include "swganh/logger.h"
#include "swganh/database/database_manager.h"

using namespace swganh::galaxy;
using namespace std;

MysqlGalaxyProvider::MysqlGalaxyProvider(swganh::database::DatabaseManager* db_manager)
    : GalaxyProviderInterface()
    , db_manager_(db_manager) {}

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

		while(statement->getMoreResults());

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return population;
}