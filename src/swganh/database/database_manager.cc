// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#define NOMINMAX

#include "swganh/database/database_manager.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <queue>

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>

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

using namespace swganh::database;
using namespace std;

class swganh::database::DatabaseManagerImpl {
public:
     
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

    explicit DatabaseManagerImpl(sql::Driver* driver)
        : driver_(driver) {}

    ~DatabaseManagerImpl() {
        std::for_each(connections_.begin(), connections_.end(), [] (ConnectionPoolMap::value_type& conn) {
            if (conn.second.empty()) {
                return;
            }

            do {
                // close each connection and pop it from the list
                std::shared_ptr<sql::Connection> connection;
                if (conn.second.try_pop(connection)) {
                    connection->close();
                }
            } while (!conn.second.empty());
        });
    }
    
    bool hasStorageType(const StorageType& storage_type) const {
        return connection_data_.find(storage_type) != connection_data_.end();
    }

    bool registerStorageType(
        const StorageType& storage_type, 
        const std::string& schema, 
        const std::string& host, 
        const std::string& username, 
        const std::string& password) 
    {
        if (hasStorageType(storage_type)) 
        {
            return false;
        }

        // create a valid connection to verify the integrity of the data passed in
        auto connection = std::shared_ptr<sql::Connection>(
            driver_->connect(host, username, password), 
            bind(&DatabaseManagerImpl::recycleConnection_, this, storage_type, placeholders::_1));

        connection->setSchema(schema);

        // insert the data
        connection_data_.insert(make_pair(storage_type, make_shared<ConnectionData>(schema, host, username, password)));

        // add the created connection to the connection pool
        connections_[storage_type].push(connection);

        return true;
    }

    bool hasConnection(const StorageType& storage_type) const {
        // return whether or not the connection pool for this storage type is empty
        auto find_iter = connections_.find(storage_type);

        if (find_iter != end(connections_))
        {
            return !find_iter->second.empty();
        }

        return false;
    }

    std::shared_ptr<sql::Connection> getConnection(const StorageType& storage_type) 
    {
        auto connection_pool_iter = connections_.find(storage_type);

        if (connection_pool_iter == connections_.end() || connection_pool_iter->second.empty())
        {
            auto connection_data_iter = connection_data_.find(storage_type);
            if (connection_data_iter == connection_data_.end())
            {
                assert(false && "Requested a storage type that has not been registered");
                return nullptr;
            }
        
            // create a valid connection to verify the integrity of the data passed in
            return CreateConnection(connection_data_iter->second, storage_type);
        }

        std::shared_ptr<sql::Connection> connection;        
        if (!connection_pool_iter->second.try_pop(connection))
        {
            return nullptr;
        }
        
        if (connection->isClosed())
        {
            return nullptr;
        }

        return connection;
    }

private:

    std::shared_ptr<sql::Connection> CreateConnection(const shared_ptr<ConnectionData>& connection_data, const StorageType& storage_type)
    {
        auto connection = std::shared_ptr<sql::Connection>(driver_->connect(connection_data->host, connection_data->username, connection_data->password), 
                std::bind(&DatabaseManagerImpl::recycleConnection_, this, storage_type, std::placeholders::_1));
        connection->setSchema(connection_data->schema);

        return connection;
    }

    void recycleConnection_(const StorageType& storage_type, sql::Connection* connection) {
        if (connection->isClosed()) 
        {
            delete connection;
            return;
        }
        
        connections_[storage_type].push(shared_ptr<sql::Connection>(
            connection, 
            bind(&DatabaseManagerImpl::recycleConnection_, this, storage_type, placeholders::_1)));
    }
       
    sql::Driver* driver_;
    
    typedef Concurrency::concurrent_unordered_map<StorageType, std::shared_ptr<ConnectionData>> ConnectionDataMap;
    ConnectionDataMap connection_data_;
    
    typedef Concurrency::concurrent_queue<std::shared_ptr<sql::Connection>> ConnectionPool;
    typedef Concurrency::concurrent_unordered_map<StorageType, ConnectionPool> ConnectionPoolMap;
    ConnectionPoolMap connections_;
};

DatabaseManager::DatabaseManager(sql::Driver* driver)   
: pimpl_(std::unique_ptr<DatabaseManagerImpl>(new DatabaseManagerImpl(driver)))
{}

DatabaseManager::~DatabaseManager() {}


bool DatabaseManager::hasStorageType(const StorageType& storage_type) const {
    return pimpl_->hasStorageType(storage_type);
}

bool DatabaseManager::registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password) {
    return pimpl_->registerStorageType(storage_type, schema, host, username, password);
}

bool DatabaseManager::hasConnection(const StorageType& storage_type) const {
    return pimpl_->hasConnection(storage_type);
}

shared_ptr<sql::Connection> DatabaseManager::getConnection(const StorageType& storage_type) {
    return pimpl_->getConnection(storage_type);
}
