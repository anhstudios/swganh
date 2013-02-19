// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "swganh/hash_string.h"

namespace swganh {
namespace database {

class DatabaseManagerImpl;

/*! An identifier used to label different persistant data storage types.
*/
typedef swganh::HashString StorageType;

/*! Exposes an API for managing mysql connector/c++ connections.
*/
class DatabaseManager : private boost::noncopyable {
public:
    /**
    * @brief Overloaded constructor taking an sql driver.
    *
    * @param driver An instance of the sql driver used to provide concrete 
    *      functionality for the database layer.
    */
    explicit DatabaseManager(sql::Driver* driver);

    ~DatabaseManager();

    /*! Check to see whether a specified storage type has been registered with
    * the DatabaseManager instance or not.
    *
    * @return bool True if the storage type has been registered, false if not.
    */
    bool hasStorageType(const StorageType& storage_type) const;
    
    /*! Registers a storage type with the DatabaseManager. This creates a connection
    * to the datastore to validate the settings, which is then immediately placed
    * in the connection pool for use.
    *
    * @return bool True if the storage type was registered, false if registration already exist.
    */
    bool registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password);
    
    /*! Check to see whether a connection exists already for a given storage type.
    *
    * @return bool True if a connection for the given storage type exists, false if not.
    */
    bool hasConnection(const StorageType& storage_type) const;
    
    /*! Processes a request for a connection to a specific storage type.
    *
    * @param storage_type The storage type a connection is being requested for.
    * @return Returns a connection or nullptr if one could not be created, or if
    * the storage type has not been seen before.
    */
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