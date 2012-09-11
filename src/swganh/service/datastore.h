// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include <list>
#include <memory>
#include <string>

#include <boost/noncopyable.hpp>

#include "swganh/service/datastore_interface.h"
#include "swganh/service/galaxy.h"

namespace sql {
    class Connection; 
}

namespace swganh {
namespace service {

class ServiceDescription;

class Datastore : public DatastoreInterface , boost::noncopyable {
public:
    explicit Datastore(std::shared_ptr<sql::Connection> connection);
    ~Datastore();
        
    std::shared_ptr<Galaxy> createGalaxy(const std::string& name, const std::string& version) const;
    std::shared_ptr<Galaxy> findGalaxyById(uint32_t id) const;
    std::shared_ptr<Galaxy> findGalaxyByName(const std::string& name) const;
    void saveGalaxyStatus(int32_t galaxy_id, int32_t status) const;
    
    bool createService(const Galaxy& galaxy, ServiceDescription& description) const;
    std::shared_ptr<ServiceDescription> findServiceById(uint32_t id) const;
    bool deleteServiceById(uint32_t id) const;
    void saveService(const ServiceDescription& service) const;
    
    std::list<Galaxy> getGalaxyList() const;
    std::list<ServiceDescription> getServiceList(uint32_t galaxy_id) const;
    std::string prepareTimestampForStorage(const std::string& timestamp) const;
private:
    Datastore();
    
    std::shared_ptr<sql::Connection> connection_;
};

}  // namespace service_directory
}  // namespace swganh

