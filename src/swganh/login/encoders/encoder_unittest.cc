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
#include "swganh/login/encoders/sha512_encoder.h"

using namespace swganh::login::encoders;

namespace {
    /*TEST(SHA512EncoderTest, MySQLGeneratesSHA512) {
        auto db_manager = std::make_shared<anh::database::DatabaseManager>(sql::mysql::get_driver_instance());
        db_manager->registerStorageType("account", "symfony", "localhost", "root", "swganh");
        Sha512Encoder encoder(db_manager);
        encoder.EncodePassword("p4$$w0rd", "0xDEADBABE");

        bool valid = encoder.IsPasswordValid("5baa36cf1a5c078d66bd97fbde40a257d960a1360316a28197e357c063bd80e90693a5cafc4e3cd1f94930abfd28f537b947d28826bc75be1b5b1f6cb799905b"
            ,"p4$$w0rd", "0xDEADBABE");

        EXPECT_TRUE(valid);
    }*/
}