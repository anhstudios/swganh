// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#define NOMINMAX

#include "swganh/database/database_manager.h"

#include <algorithm>
#include <functional>

#include <cppconn/driver.h>

using namespace swganh::database;

DatabaseManager::DatabaseManager(sql::Driver* driver, uint32_t num_threads)   
    : driver_(driver)
    , thread_pool_(num_threads)
{}

DatabaseManager::~DatabaseManager() 
{
    std::for_each(connections_.begin(), connections_.end(), [] (ConnectionPoolMap::value_type& conn) {
        while (!conn.second.empty()) 
        {
            // close each connection and pop it from the list
            std::shared_ptr<sql::Connection> connection;
            if (conn.second.try_pop(connection)) {
                connection->close();
            }
        }
    });
}


bool DatabaseManager::hasStorageType(const StorageType& storage_type) const {
    return connection_data_.find(storage_type) != connection_data_.end();
}

bool DatabaseManager::registerStorageType(const StorageType& storage_type, const std::string& schema, const std::string& host, const std::string& username, const std::string& password) {
    if (hasStorageType(storage_type)) 
    {
        return false;
    }

    // create a valid connection to verify the integrity of the data passed in
    auto connection = std::shared_ptr<sql::Connection>(
        driver_->connect(host, username, password), 
        std::bind(&DatabaseManager::recycleConnection_, this, storage_type, std::placeholders::_1));

    connection->setSchema(schema);

    // insert the data
    connection_data_.insert(std::make_pair(storage_type, std::make_shared<ConnectionData>(schema, host, username, password)));

    // add the created connection to the connection pool
    connections_[storage_type].push(connection);

    return true;
}

bool DatabaseManager::hasConnection(const StorageType& storage_type) const 
{
    // return whether or not the connection pool for this storage type is empty
    auto find_iter = connections_.find(storage_type);

    if (find_iter != end(connections_))
    {
        return !find_iter->second.empty();
    }

    return false;
}

std::shared_ptr<sql::Connection> DatabaseManager::getConnection(const StorageType& storage_type) 
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

std::shared_ptr<sql::Connection> DatabaseManager::CreateConnection(const std::shared_ptr<ConnectionData>& connection_data, const StorageType& storage_type)
{
    auto connection = std::shared_ptr<sql::Connection>(driver_->connect(connection_data->host, connection_data->username, connection_data->password), 
            std::bind(&DatabaseManager::recycleConnection_, this, storage_type, std::placeholders::_1));
    connection->setSchema(connection_data->schema);

    return connection;
}

void DatabaseManager::recycleConnection_(const StorageType& storage_type, sql::Connection* connection) {
    if (connection->isClosed()) 
    {
        delete connection;
        return;
    }
    
    connections_[storage_type].push(std::shared_ptr<sql::Connection>(
        connection, 
        std::bind(&DatabaseManager::recycleConnection_, this, storage_type, std::placeholders::_1)));
}