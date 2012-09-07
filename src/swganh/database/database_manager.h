// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "database_manager_interface.h"

namespace swganh {
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
}  // namespace swganh