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

#ifndef ANH_DATABASE_MOCK_DATABASE_MANAGER_H_
#define ANH_DATABASE_MOCK_DATABASE_MANAGER_H_

#include <gmock/gmock.h>
#include <anh/database/database_manager.h>

namespace anh {
namespace database{

class MockDatabaseManager : public IDatabaseManager
{
public:
    MOCK_CONST_METHOD1(hasStorageType, bool(const StorageType& storage_type));
    MOCK_METHOD5(registerStorageType, bool(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password));
    MOCK_CONST_METHOD1(hasConnection, bool(const StorageType& storage_type));
    MOCK_METHOD1(getConnection, std::shared_ptr<sql::Connection>(const StorageType& storage_type));    
};
} //namespace database
} //namespace anh

#endif //ANH_DATABASE_MOCK_DATABASE_MANAGER_H_