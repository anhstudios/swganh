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
#ifndef ANH_DATABASE_BASE_DB_UPDATER_H_
#define ANH_DATABASE_BASE_DB_UPDATER_H_

#include <anh/database/database_manager_interface.h>

namespace sql { class ResultSet; }
namespace anh { namespace database {
template<class ClassToMap>
class BaseDBMapper
{
public:
	BaseDBMapper(std::shared_ptr<database::DatabaseManagerInterface> db_manager) { db_manager_ = db_manager; }
	virtual void persist(std::shared_ptr<ClassToMap> ref_class) = 0;
	virtual std::shared_ptr<ClassToMap> query_result(uint64_t entity_id) = 0;
	std::shared_ptr<database::DatabaseManagerInterface> db_manager() { return db_manager_; }
protected:
	std::shared_ptr<database::DatabaseManagerInterface> db_manager_;
    // the last ms the data was persisted
    uint32_t last_updated_;
    // the interval do you want the data to persist in ms
    uint32_t update_threshold_;
};
} // database
} // anh


#endif // ANH_DATABASE_BASE_DB_UPDATER_H_
