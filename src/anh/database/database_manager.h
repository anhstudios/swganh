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

#include <cstdint>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#include "anh/hash_string.h"

namespace sql {
    class Connection;
    class Driver;
}

namespace anh {
namespace database {
    
/*! An identifier used to label different persistant data storage types.
*/
typedef anh::HashString StorageType;

/*! Interface class that exposes an API for managing mysql connector/c++
* connections.
*/
class IDatabaseManager {
public:    
    virtual ~IDatabaseManager() {}

    /*! Check to see whether a specified storage type has been registered with 
    * the DatabaseManager instance or not.
    *
    * \return bool True if the storage type has been registered, false if not.
    */
    virtual bool hasStorageType(const StorageType& storage_type) const = 0;

    /*! Registers a storage type with the DatabaseManager. This creates a connection
    * to the datastore to validate the settings, which is then immediately placed
    * in the connection pool for use.
    *
    * \return bool True if the storage type was registered, false if registration already exist.
    */
    virtual bool registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password) = 0;

    /*! Check to see whether a connection exists already for a given storage type.
    *
    * \return bool True if a connection for the given storage type exists, false if not.
    */
    virtual bool hasConnection(const StorageType& storage_type) const = 0;

    /*! Processes a request for a connection to a specific storage type. 
    *
    * \param storage_type The storage type a connection is being requested for.
    * \return Returns a connection or nullptr if one could not be created, or if
    *   the storage type has not been seen before.
    */
    virtual std::shared_ptr<sql::Connection> getConnection(const StorageType& storage_type) = 0;
};


class DatabaseManagerImpl;

/*! Concrete implementation of the IDatabaseManager API.
*
* \see IDatabaseManager
*/
class DatabaseManager : public IDatabaseManager, private boost::noncopyable {
public:
    /**
    * \brief Overloaded constructor taking an sql driver.
    *
    * \param driver An instance of the sql driver used to provide concrete 
    *      functionality for the database layer.
    */
    explicit DatabaseManager(sql::Driver* driver);

    ~DatabaseManager();

    /// \see IDatabaseManager::hasStorageType
    bool hasStorageType(const StorageType& storage_type) const;
    
    /// \see IDatabaseManager::registerStorageType
    bool registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password);
    
    /// \see IDatabaseManager::hasConnection
    bool hasConnection(const StorageType& storage_type) const;
    
    /// \see IDatabaseManager::getConnection
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
