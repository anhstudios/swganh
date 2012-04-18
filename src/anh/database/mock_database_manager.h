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

#include <turtle/mock.hpp>
#include <anh/database/database_manager_interface.h>

namespace anh {
namespace database{

MOCK_BASE_CLASS(MockDatabaseManager, DatabaseManagerInterface)
{
    MOCK_CONST_METHOD_EXT(hasStorageType, 1, bool(const StorageType& storage_type), hasStorageType);
    MOCK_METHOD(registerStorageType, 5);
    MOCK_CONST_METHOD_EXT(hasConnection, 1, bool(const StorageType& storage_type), hasConnection);
    MOCK_METHOD(getConnection, 1);    
};
} //namespace database
} //namespace anh

#endif //ANH_DATABASE_MOCK_DATABASE_MANAGER_H_