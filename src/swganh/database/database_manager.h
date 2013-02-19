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

#ifdef WIN32
#include <concurrent_unordered_map.h>
#include <concurrent_queue.h>
#else
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_queue.h>

namespace Concurrency {
    using ::tbb::concurrent_unordered_map;
    using ::tbb::concurrent_queue;
}
#endif

#include "swganh/hash_string.h"

namespace swganh {
namespace database {

class DatabaseManagerImpl;

/*! An identifier used to label different persistant data storage types.
*/
typedef swganh::HashString StorageType;

struct ConnectionData {
    ConnectionData(std::string schema_, std::string host_, std::string username_, std::string password_)
        : schema(std::move(schema_))
        , host(std::move(host_))
        , username(std::move(username_))
        , password(std::move(password_))
    {}

    std::string schema;
    std::string host;
    std::string username;
    std::string password;
};

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
    
    std::shared_ptr<sql::Connection> CreateConnection(const std::shared_ptr<ConnectionData>& connection_data, const StorageType& storage_type);
    
    void recycleConnection_(const StorageType& storage_type, sql::Connection* connection);

    sql::Driver* driver_;
    
    typedef Concurrency::concurrent_unordered_map<StorageType, std::shared_ptr<ConnectionData>> ConnectionDataMap;
    ConnectionDataMap connection_data_;
    
    typedef Concurrency::concurrent_queue<std::shared_ptr<sql::Connection>> ConnectionPool;
    typedef Concurrency::concurrent_unordered_map<StorageType, ConnectionPool> ConnectionPoolMap;
    ConnectionPoolMap connections_;
};

}  // namespace database
}  // namespace swganh