// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_DATABASE_MOCK_DATABASE_MANAGER_H_
#define ANH_DATABASE_MOCK_DATABASE_MANAGER_H_

#include <turtle/mock.hpp>
#include <swganh/database/database_manager_interface.h>

namespace swganh {
namespace database{

MOCK_BASE_CLASS(MockDatabaseManager, DatabaseManagerInterface)
{
    MOCK_CONST_METHOD_EXT(hasStorageType, 1, bool(const StorageType& storage_type), hasStorageType);
    MOCK_METHOD(registerStorageType, 5);
    MOCK_CONST_METHOD_EXT(hasConnection, 1, bool(const StorageType& storage_type), hasConnection);
    MOCK_METHOD(getConnection, 1);    
};
} //namespace database
} //namespace swganh

#endif //ANH_DATABASE_MOCK_DATABASE_MANAGER_H_