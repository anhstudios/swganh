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

#include "anh/server_directory/datastore.h"

#include <memory>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "anh/server_directory/galaxy.h"
#include "anh/server_directory/service.h"

using namespace anh::server_directory;
using namespace boost::posix_time;
using namespace std;

Datastore::Datastore(shared_ptr<sql::Connection> connection) 
    : connection_(connection)
{}

Datastore::~Datastore() {}

std::shared_ptr<Galaxy> Datastore::findGalaxyByName(const std::string& name) const {
    std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "SELECT * FROM cluster "
        "WHERE name = ? LIMIT 1"));

    statement->setString(1, name);

    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

    // if no results are found return a nullptr
    if (!result->next()) {
        return nullptr;
    }

    auto cluster = make_shared<Galaxy>(
        result->getUInt("id"),
        result->getUInt("primary_id"),
        result->getString("name"),
        result->getString("version"),
        static_cast<Galaxy::StatusType>(result->getInt("status")),
        result->getString("created_at"),
        result->getString("updated_at"));

    return cluster;
}

std::shared_ptr<Galaxy> Datastore::createGalaxy(
    const std::string& name,
    const std::string& version) const 
{
    std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "INSERT INTO galaxy(name, version, created_at, updated_at) "
        "VALUES(?, ?, NOW(), NOW())"));

    statement->setString(1, name);
    statement->setString(2, version);

    // if the statement fails to process return a nullptr
    if (statement->executeUpdate() <= 0) {
        return nullptr;
    }

    statement.reset(connection_->prepareStatement(
        "SELECT * FROM galaxy WHERE id = LAST_INSERT_ID()"));
    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

    if (!result->next()) {
        return nullptr;
    }

    auto cluster = make_shared<Galaxy>(
        result->getUInt("id"),
        result->getUInt("primary_id"),
        result->getString("name"),    
        result->getString("version"),
        static_cast<Galaxy::StatusType>(result->getInt("status")),
        result->getString("created_at"),
        result->getString("updated_at"));

    return cluster;
}

std::shared_ptr<Service> Datastore::createService(std::shared_ptr<Galaxy> cluster, const std::string& name, const std::string& type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping_port) const {

    std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "INSERT INTO service (galaxy_id, "
                             "name, "
                             "type, "
                             "version, "
                             "address, "
                             "tcp_port, "
                             "udp_port, "
                             "ping_port, "
                             "status, "
                             "last_pulse, "
                             "created_at, "
                             "updated_at) "
        "VALUES(?, ?, ?, ?, INET_ATON(?), ?, ?, ?, ?, NOW(), NOW(), NOW())"));

    uint32_t cluster_id = 0;
    if (cluster) {
        cluster_id = cluster->id();
    }

    statement->setInt(1, cluster_id);
    statement->setString(2, name);
    statement->setString(3, type);
    statement->setString(4, version);
    statement->setString(5, address);
    statement->setUInt(6, static_cast<uint32_t>(tcp_port));
    statement->setUInt(7, static_cast<uint32_t>(udp_port));
    statement->setUInt(8, static_cast<uint32_t>(ping_port));
    statement->setInt(9, static_cast<int32_t>(-1));
    
    if (! (statement->executeUpdate() > 0)) {
        return nullptr;
    }
    
    statement.reset(connection_->prepareStatement(
        "SELECT id, galaxy_id, name, type, version, INET_NTOA(address) as address_string, tcp_port, udp_port, ping_port, status, CONVERT(TIMESTAMP(last_pulse), CHAR) as last_pulse_timestamp "
        "FROM service WHERE id = LAST_INSERT_ID()"));
    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
    
    if (!result->next()) {
        return nullptr;
    }
    
    auto proc = make_shared<Service>(
        result->getUInt("id"),
        result->getUInt("galaxy_id"),
        result->getString("name"),
        result->getString("type"),
        result->getString("version"),
        result->getString("address_string"),
        result->getUInt("tcp_port"),
        result->getUInt("udp_port"),
        result->getUInt("ping_port")
        );
    
    proc->status(result->getInt("status"));
    proc->last_pulse(result->getString("last_pulse_timestamp"));

    return proc;
}

std::string Datastore::getGalaxyTimestamp(std::shared_ptr<Galaxy> cluster) const {
    auto process = findServiceById(cluster->primary_id());

    if (!process) {
        return "";
    }

    return process->last_pulse();
}

void Datastore::saveService(std::shared_ptr<Service> process) const {
    std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "UPDATE service SET address = INET_ATON(?), tcp_port = ?, udp_port = ?, ping_port = ?, status = ?, last_pulse = ? WHERE id = ?"));
    
    statement->setString(1, process->address());
    statement->setUInt(2, process->tcp_port());
    statement->setUInt(3, process->udp_port());
    statement->setUInt(4, process->ping_port());
    statement->setInt(5, process->status());
    statement->setString(6, prepareTimestampForStorage(process->last_pulse()));
    statement->setUInt(7, process->id());
    statement->executeUpdate();
}

std::shared_ptr<Galaxy> Datastore::findGalaxyById(uint32_t id) const {
     std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "SELECT * FROM galaxy "
        "WHERE id = ? LIMIT 1"));

    statement->setUInt(1, id);

    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

    // if the statement fails to process return a nullptr
    if (!result->next()) {
        return nullptr;
    }

    auto cluster = make_shared<Galaxy>(
        result->getUInt("id"),
        result->getUInt("primary_id"),
        result->getString("name"),
        result->getString("version"),
        static_cast<Galaxy::StatusType>(result->getInt("status")),
        result->getString("created_at"),
        result->getString("updated_at"));

    return cluster;
}

std::shared_ptr<Service> Datastore::findServiceById(uint32_t id) const {
     std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "SELECT * FROM service "
        "WHERE id = ? LIMIT 1"));

    statement->setUInt(1, id);

    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

    // if the statement fails to process return a nullptr
    if (!result->next()) {
        return nullptr;
    }
        
    auto proc = make_shared<Service>(
        result->getUInt("id"),
        result->getUInt("galaxy_id"),
        result->getString("name"),
        result->getString("type"),
        result->getString("version"),
        result->getString("address_string"),
        result->getUInt("tcp_port"),
        result->getUInt("udp_port"),
        result->getUInt("ping_port")
        );

    proc->status(result->getInt("status"));
    proc->last_pulse(result->getString("last_pulse_timestamp"));

    return proc;
}

bool Datastore::deleteServiceById(uint32_t id) const {
    std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "DELETE FROM service WHERE id = ? "
    ));

    statement->setUInt(1, id);

    return statement->executeUpdate() > 0;
}

list<Galaxy> Datastore::getGalaxyList() const {
    std::unique_ptr<sql::Statement> statement(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> result(statement->executeQuery(
        "SELECT * FROM galaxy ORDER BY galaxy.name"));

    // Loop through the results and create a map entry for each.
    std::list<Galaxy> cluster_list;
    uint32_t id = 0;

    while (result->next()) {
        id = result->getUInt("id");

        cluster_list.push_back(Galaxy(
            id,
            result->getUInt("primary_id"),
            result->getString("name"),
            result->getString("version"),
            static_cast<Galaxy::StatusType>(result->getInt("status")),
            result->getString("created_at"),
            result->getString("updated_at")));
    }

    return cluster_list;
}

list<Service> Datastore::getServiceList(uint32_t cluster_id) const {
    std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
        "SELECT id, galaxy_id, type, version, address, tcp_port, udp_port, status, TIMESTAMP(last_pulse) as last_pulse_timestamp "
        "FROM service WHERE galaxy_id = ? ORDER BY service.type"));

    statement->setUInt(1, cluster_id);
    
    std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

    // Loop through the results and create a map entry for each.
    std::list<Service> process_list;
    uint32_t id = 0;

    while (result->next()) {
        id = result->getUInt("id");

        Service proc(
            id,
            result->getUInt("galaxy_id"),
            result->getString("name"),
            result->getString("type"),
            result->getString("version"),
            result->getString("address"),
            result->getUInt("tcp_port"),
            result->getUInt("udp_port"),
            result->getUInt("ping_port"));
        proc.status(result->getInt("status"));
        proc.last_pulse(result->getString("last_pulse_timestamp"));
        
        process_list.push_back(std::move(proc));
    }

    return process_list;
}

std::string Datastore::prepareTimestampForStorage(const std::string& timestamp) const {    
    std::stringstream ss;
    
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet("%Y%m%d%H%M%S%F");    
    ss.imbue(std::locale(ss.getloc(), facet));
    
    ss << boost::posix_time::time_from_string(timestamp);
    
    return ss.str();
}
