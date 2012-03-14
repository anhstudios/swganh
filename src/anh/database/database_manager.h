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

#ifndef ANH_DATABASE_DATABASE_MANAGER_H_
#define ANH_DATABASE_DATABASE_MANAGER_H_

#include "database_manager_interface.h"

namespace anh {
namespace database {
class DatabaseManagerImpl;

/*! Concrete implementation of the DatabaseManagerInterface API.
*
* @see DatabaseManagerInterface
*/
class DatabaseManager : public DatabaseManagerInterface, private boost::noncopyable {
public:
    /**
    * @brief Overloaded constructor taking an sql driver.
    *
    * @param driver An instance of the sql driver used to provide concrete 
    *      functionality for the database layer.
    */
    explicit DatabaseManager(sql::Driver* driver);

    ~DatabaseManager();

    /// @see DatabaseManagerInterface::hasStorageType
    bool hasStorageType(const StorageType& storage_type) const;
    
    /// @see DatabaseManagerInterface::registerStorageType
    bool registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password);
    
    /// @see DatabaseManagerInterface::hasConnection
    bool hasConnection(const StorageType& storage_type) const;
    
    /// @see DatabaseManagerInterface::getConnection
    std::shared_ptr<sql::Connection> getConnection(const StorageType& storage_type);
    
private:
    // disable the default constructor to ensure that DatabaseManager is always
    // created with a driver instance
    DatabaseManager();

    // private implementation of the database manager internals make it easier
    // to modify how the connection pooling works under the hood without affecting
    // users of the api
    std::unique_ptr<DatabaseManagerImpl> pimpl_;
};

}  // namespace database
}  // namespace anh

#endif  // ANH_DATABASE_DATABASE_MANAGER_H_
