/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "transform_db_mapper.h"
#include <anh/database/database_manager.h>
#include <mod_anh_transform/transform_component.h>
#include "main.h"
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

using namespace std;
using namespace anh::api::components;

namespace transform {
void TransformDBMapper::persist(std::shared_ptr<TransformComponentInterface> component)
{
    if (component != nullptr)
    {
        auto conn = gDatabaseManager->getConnection("galaxy");
        sql::SQLString query_string("update transform set x = ?, y = ?, z = ?, rW = ?, rX = ?, rY = ?, rZ = ?, parent_id = ?");
        auto prepared = 
            unique_ptr<sql::PreparedStatement>(conn->prepareStatement(query_string));
        // set the values in place of ?
        prepared->setDouble(0, component->position().x); prepared->setDouble(1, component->position().y);
        prepared->setDouble(2, component->position().z); prepared->setDouble(3, component->rotation().w);
        prepared->setDouble(4, component->rotation().x); prepared->setDouble(5, component->rotation().y);
        prepared->setDouble(6, component->rotation().z); prepared->setInt64(7, component->parent_id());
        prepared->executeQuery();
    }
}
std::shared_ptr<TransformComponentInterface> TransformDBMapper::query_result(uint64_t entity_id)
{
	auto conn = gDatabaseManager->getConnection("galaxy");
    sql::SQLString query_string("select * from transform where entity_id = ?");
    auto prepared = 
            unique_ptr<sql::PreparedStatement>(conn->prepareStatement(query_string));
	prepared->setInt64(0, entity_id);

	auto result = unique_ptr<sql::ResultSet>(prepared->executeQuery());
	auto transform = make_shared<TransformComponent>();
	transform->position(result->getDouble("x"), result->getDouble("y"), result->getDouble("z"));
	transform->rotation(result->getDouble("rX"), result->getDouble("rY"), result->getDouble("rZ"), result->getDouble("rW"));
	
	return transform;
}

} // transform