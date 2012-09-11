// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>
#include "swganh/hash_string.h"

namespace sql {
    class Connection;
    class Driver;
}
  
/*! An identifier used to label different persistant data storage types.
*/
typedef swganh::HashString StorageType;

namespace swganh {
namespace database {
/*! Interface class that exposes an API for managing mysql connector/c++
* connections.
*/
class DatabaseManagerInterface {
public:    
    virtual ~DatabaseManagerInterface() {}

    /*! Check to see whether a specified storage type has been registered with 
    * the DatabaseManager instance or not.
    *
    * @return bool True if the storage type has been registered, false if not.
    */
    virtual bool hasStorageType(const StorageType& storage_type) const = 0;

    /*! Registers a storage type with the DatabaseManager. This creates a connection
    * to the datastore to validate the settings, which is then immediately placed
    * in the connection pool for use.
    *
    * @return bool True if the storage type was registered, false if registration already exist.
    */
    virtual bool registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password) = 0;

    /*! Check to see whether a connection exists already for a given storage type.
    *
    * @return bool True if a connection for the given storage type exists, false if not.
    */
    virtual bool hasConnection(const StorageType& storage_type) const = 0;

    /*! Processes a request for a connection to a specific storage type. 
    *
    * @param storage_type The storage type a connection is being requested for.
    * @return Returns a connection or nullptr if one could not be created, or if
    *   the storage type has not been seen before.
    */
    virtual std::shared_ptr<sql::Connection> getConnection(const StorageType& storage_type) = 0;
};
} // database
} // anh