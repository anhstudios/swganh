/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

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

#include <gtest/gtest.h>

#include "swganh/object/tangible/tangible.h"
#include "swganh/object/tangible/tangible_factory.h"
#include "swganh/object/object_manager.h"
#include "anh/database/database_manager.h"

#include <mysql_driver.h>
#include <cppconn/driver.h>

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::tangible;

namespace {
    
TEST(ObjectLoad, CanLoadObject) {
//    auto object_manager = make_shared<ObjectManager>();
//    // create db manager
//    
//    auto db_manager = std::make_shared<anh::database::DatabaseManager>(sql::mysql::get_driver_instance());
//    db_manager->registerStorageType("galaxy", "galaxy", "localhost", "root", "swganh");
//    auto tangible_factory = make_shared<TangibleFactory>(db_manager);
//    object_manager->RegisterObjectType(Tangible::type, tangible_factory);
//    auto tangible = object_manager->CreateObjectFromStorage<Tangible>(0);
//    ASSERT_EQ(0, tangible->GetObjectId());
}

}  // namespace

