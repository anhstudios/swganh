 // This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/service/datastore.h"

#include <memory>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "swganh/service/service_description.h"
#include "swganh/logger.h"

using namespace swganh::service;
using namespace boost::posix_time;
using namespace std;

Datastore::Datastore(shared_ptr<sql::Connection> connection) 
    : connection_(connection)
{}

Datastore::~Datastore() {}

std::shared_ptr<Galaxy> Datastore::findGalaxyByName(const std::string& name) const {
    shared_ptr<Galaxy> galaxy = nullptr;

    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
            "CALL sp_FindGalaxyByName(?);"));

        statement->setString(1, name);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        // if no results are found return a nullptr
        if (!result->next()) {
			statement->getMoreResults();
            return nullptr;
        }

        galaxy = make_shared<Galaxy>(
            result->getUInt("id"),
            result->getUInt("primary_id"),
            result->getString("name"),
            result->getString("version"),
            static_cast<Galaxy::StatusType>(result->getInt("status")),
            result->getString("created_at"),
            result->getString("updated_at"));
		
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return galaxy;
}

std::shared_ptr<Galaxy> Datastore::createGalaxy(
    const std::string& name,
    const std::string& version) const 
{
    shared_ptr<Galaxy> galaxy = nullptr;

    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
            "CALL sp_CreateGalaxy(?,?,?);"));

        statement->setString(1, name);
        statement->setString(2, version);
        statement->setInt(3, 0);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        if (!result->next()) {
			statement->getMoreResults();
            return nullptr;
        }

        galaxy = make_shared<Galaxy>(
            result->getUInt("id"),
            result->getUInt("primary_id"),
            result->getString("name"),    
            result->getString("version"),
            static_cast<Galaxy::StatusType>(result->getInt("status")),
            result->getString("created_at"),
            result->getString("updated_at"));

		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return galaxy;
}
void Datastore::saveGalaxyStatus(int32_t galaxy_id, int32_t status) const
{
    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
			"CALL sp_UpdateGalaxyStatus(?,?);"));

        statement->setInt(1, status);
        statement->setInt(2, galaxy_id);

        statement->executeUpdate();

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

bool Datastore::createService(const Galaxy& galaxy, ServiceDescription& description) const {
    shared_ptr<ServiceDescription> service = nullptr;

    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
            "CALL sp_CreateService(?,?,?,?,?,?,?,?,?);"));
        
        uint32_t galaxy_id = galaxy.id();

        statement->setInt(1, galaxy_id);
        statement->setString(2, description.name());
        statement->setString(3, description.type());
        statement->setString(4, description.version());
        statement->setString(5, description.address());
        statement->setUInt(6, static_cast<uint32_t>(description.tcp_port()));
        statement->setUInt(7, static_cast<uint32_t>(description.udp_port()));
        statement->setUInt(8, static_cast<uint32_t>(description.ping_port()));
        statement->setInt(9, static_cast<int32_t>(-1));
        
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
        
        if (!result->next()) {
            return false;
        }
    
        description.id(result->getUInt("id"));
        description.status(result->getInt("status"));
        description.last_pulse(result->getString("last_pulse_timestamp"));

		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        return false;
    }

    return true;
}

void Datastore::saveService(const ServiceDescription& service) const {
    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
            "CALL sp_SaveService(?,?,?,?,?,?,?);"));
        
        statement->setString(1, service.address());
        statement->setUInt(2, service.tcp_port());
        statement->setUInt(3, service.udp_port());
        statement->setUInt(4, service.ping_port());
        statement->setInt(5, service.status());
        statement->setString(6, prepareTimestampForStorage(service.last_pulse()));
        statement->setUInt(7, service.id());
        statement->executeUpdate();
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

std::shared_ptr<Galaxy> Datastore::findGalaxyById(uint32_t id) const {
    std::shared_ptr<Galaxy> galaxy = nullptr;

    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
            "CALL sp_FindGalaxyById(?);"));

        statement->setUInt(1, id);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        // if the statement fails to service return a nullptr
        if (!result->next()) {
			while(statement->getMoreResults())
                ;
            return nullptr;
        }

        galaxy = make_shared<Galaxy>(
            result->getUInt("id"),
            result->getUInt("primary_id"),
            result->getString("name"),
            result->getString("version"),
            static_cast<Galaxy::StatusType>(result->getInt("status")),
            result->getString("created_at"),
            result->getString("updated_at"));

		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return galaxy;
}

std::shared_ptr<ServiceDescription> Datastore::findServiceById(uint32_t id) const {
    shared_ptr<ServiceDescription> service = nullptr;

    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
            "CALL sp_FindServiceById(?);"));

        statement->setUInt(1, id);

        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());

        // if the statement fails to service return a nullptr
        if (result->next()) {                
            service = make_shared<ServiceDescription>(
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

            service->status(result->getInt("status"));
            service->last_pulse(result->getString("last_pulse_timestamp"));
        }
		while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return service;
}

bool Datastore::deleteServiceById(uint32_t id) const {
    bool deleted = false;

    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
			"CALL sp_DeleteService(?); "
        ));

        statement->setUInt(1, id);

        deleted = statement->executeUpdate() > 0;

    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return deleted;
}

list<Galaxy> Datastore::getGalaxyList() const {
    std::list<Galaxy> galaxy_list;
    
    try {
        std::unique_ptr<sql::Statement> statement(connection_->createStatement());
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery(
            "CALL sp_GetGalaxyList();"));

        // Loop through the results and create a map entry for each.
        while (result->next()) {
            galaxy_list.push_back(Galaxy(
                result->getUInt("id"),
                result->getUInt("primary_id"),
                result->getString("name"),
                result->getString("version"),
                static_cast<Galaxy::StatusType>(result->getInt("status")),
                result->getString("created_at"),
                result->getString("updated_at")));
        }while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return galaxy_list;
}

list<ServiceDescription> Datastore::getServiceList(uint32_t galaxy_id) const { 
    std::list<ServiceDescription> service_list;

    try {
        std::unique_ptr<sql::PreparedStatement> statement(connection_->prepareStatement(
            "CALL sp_GetServiceList(?);"));

        statement->setUInt(1, galaxy_id);
        
        std::unique_ptr<sql::ResultSet> result(statement->executeQuery());
                   
        // Loop through the results and create a map entry for each.
        while (result->next()) {
            ServiceDescription service(
                result->getUInt("id"),
                result->getUInt("galaxy_id"),
                result->getString("name"),
                result->getString("type"),
                result->getString("version"),
                result->getString("address"),
                result->getUInt("tcp_port"),
                result->getUInt("udp_port"),
                result->getUInt("ping_port"));
            service.status(result->getInt("status"));
            service.last_pulse(result->getString("last_pulse_timestamp"));
            
            service_list.push_back(move(service));
        } while(statement->getMoreResults());
    } catch(sql::SQLException &e) {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }

    return service_list;
}

std::string Datastore::prepareTimestampForStorage(const std::string& timestamp) const {    
    std::stringstream ss;
    
    ss.imbue(std::locale(ss.getloc(), new boost::posix_time::time_facet("%Y%m%d%H%M%S%F")));
    
    static boost::mutex mutex;

    boost::lock_guard<boost::mutex> lk(mutex);

    ss << boost::posix_time::time_from_string(timestamp);
    
    return ss.str();
}
