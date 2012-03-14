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

#include <gtest/gtest.h>

#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/driver.h>

#include "anh/database/database_manager.h"
#include "anh/utilities.h"
#include "swganh/login/providers/mysql_account_provider.h"

using namespace swganh::login::providers;

namespace {
    TEST(MySQLAccountProvider, DISABLED_ProperAccountInformationReturned) {
        auto db_manager = std::make_shared<anh::database::DatabaseManager>(sql::mysql::get_driver_instance());
        db_manager->registerStorageType("account", "symfony", "localhost", "root", "swganh");
        MysqlAccountProvider account_provider(db_manager.get());
        std::shared_ptr<swganh::login::Account> account;
        account = account_provider.FindByUsername("Kronos");

        EXPECT_EQ("Kronos", account->username());        
    }
}