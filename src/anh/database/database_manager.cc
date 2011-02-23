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

#define NOMINMAX

#include "anh/database/database_manager.h"

#include <queue>

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>

#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_queue.h>

#include "anh/memory.h"

using namespace anh::database;
using namespace std;

class anh::database::DatabaseManagerImpl {
public:
    explicit DatabaseManagerImpl(sql::Driver* driver)
        : driver_(driver) {}

    ~DatabaseManagerImpl() {
        std::for_each(connections_.begin(), connections_.end(), [] (ConnectionPoolMap::value_type& conn) {
            if (conn.second.empty()) {
                return;
            }

            do {
                // close each connection and pop it from the list
                auto connection = conn.second.front();
                connection->close();
                conn.second.pop();
            } while (!conn.second.empty());
        });
    }
    
    bool hasStorageType(const StorageType& storage_type) const {
        ConnectionDataMap::const_accessor a;
        return connection_data_.find(a, storage_type);
    }

    bool registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password) {
        if (hasStorageType(storage_type)) {
            return false;
        }

        // create a valid connection to verify the integrity of the data passed in
        auto connection = std::shared_ptr<sql::Connection>(driver_->connect(host, username, password), 
            std::bind(&DatabaseManagerImpl::recycleConnection_, this, storage_type, std::placeholders::_1));
        connection->setSchema(schema);

        // insert the data
        ConnectionDataMap::accessor data_accessor;    
        connection_data_.insert(data_accessor, storage_type);
        data_accessor->second = make_shared<ConnectionData>(schema, host, username, password);

        // add the created connection to the connection pool
        ConnectionPoolMap::accessor pool_accessor;
        connections_.insert(pool_accessor, storage_type);
        pool_accessor->second.push(connection);

        return true;
    }

    bool hasConnection(const StorageType& storage_type) const {
        ConnectionPoolMap::const_accessor a;

        // return false if unable to find a connection pool for the requested type
        if (!connections_.find(a, storage_type)) {
            return false;
        }

        // return whether or not the connection pool for this storage type is empty
        return !a->second.empty();
    }

    std::shared_ptr<sql::Connection> getConnection(const StorageType& storage_type) {        
        ConnectionPoolMap::accessor a;

        // return false if unable to find a connection pool for the requested type
        if (!connections_.find(a, storage_type) || a->second.empty()) {
            // lookup the connection data for the requested storage type and fail out
            // if not found
            ConnectionDataMap::accessor data_accessor;
            if (!connection_data_.find(data_accessor, storage_type)) {
                assert(false && "Requested a storage type that has not been registered");
                return nullptr;
            }
            
            // create a valid connection to verify the integrity of the data passed in
            auto connection_data = data_accessor->second;
            auto connection = std::shared_ptr<sql::Connection>(driver_->connect(connection_data->host, connection_data->username, connection_data->password), 
                std::bind(&DatabaseManagerImpl::recycleConnection_, this, storage_type, std::placeholders::_1));
            connection->setSchema(connection_data->schema);

            return connection;
        }
        
        auto connection = a->second.front();
        a->second.pop();

        return connection;
    }

private:

    void recycleConnection_(const StorageType& storage_type, sql::Connection* connection) {
        if (connection->isClosed()) {
            delete connection;
            return;
        }
        
        ConnectionPoolMap::accessor pool_accessor;
        connections_.insert(pool_accessor, storage_type);
        pool_accessor->second.push(std::shared_ptr<sql::Connection>(connection, std::bind(&DatabaseManagerImpl::recycleConnection_, this, storage_type, std::placeholders::_1)));
    }
        
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

    sql::Driver* driver_;
    
    typedef tbb::concurrent_hash_map<StorageType, std::shared_ptr<ConnectionData>> ConnectionDataMap;
    ConnectionDataMap connection_data_;
    
    typedef std::queue<std::shared_ptr<sql::Connection>> ConnectionPool;
    typedef tbb::concurrent_hash_map<StorageType, ConnectionPool> ConnectionPoolMap;
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
