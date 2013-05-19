// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "building_factory.h"

#include "building.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"

using namespace std;
using namespace swganh::object;

BuildingFactory::BuildingFactory(swganh::app::SwganhKernel* kernel)
	: TangibleFactory(kernel)
{}

void BuildingFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    TangibleFactory::LoadFromStorage(connection, object, lock);
}

std::shared_ptr<swganh::object::Object> BuildingFactory::CreateObject()
{
	return std::make_shared<Building>();
}
