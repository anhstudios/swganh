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
#ifndef MOD_ANH_TRANSFORM_TRANFORM_DB_MAPPER_H
#define MOD_ANH_TRANSFORM_TRANFORM_DB_MAPPER_H
#include <anh/database/base_db_mapper.h>
#include <mod_anh_transform/transform_component.h>

namespace transform {

class TransformDBMapper : public anh::database::BaseDBMapper
{
public:
	TransformDBMapper(std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager);
	//virtual void persist(std::shared_ptr<void> ref);
	//virtual std::shared_ptr<sql::ResultSet> query_result(uint64_t entity_id);
};

} // transform


#endif //MOD_ANH_TRANSFORM_TRANFORM_DB_MAPPER_H