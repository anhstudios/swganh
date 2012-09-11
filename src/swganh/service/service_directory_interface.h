// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include <list>
#include <memory>
#include <string>

#include "swganh/service/galaxy.h"
#include "swganh/service/service_description.h"

namespace swganh {
namespace service {

typedef std::list<Galaxy> GalaxyList;
typedef std::list<ServiceDescription> ServiceList;

/// Simple interface
class ServiceDirectoryInterface {
public:
    virtual ~ServiceDirectoryInterface() {}

    virtual void joinGalaxy(const std::string& galaxy_name, const std::string& version = "", bool create_galaxy = false) = 0;

    virtual void updateGalaxyStatus() = 0;

    virtual bool registerService(ServiceDescription& service) = 0;
    
    virtual bool removeService(const ServiceDescription& service) = 0;
    
    virtual void updateService(const ServiceDescription& service) = 0;
    virtual void updateServiceStatus(int32_t new_status) = 0;
    
    virtual bool makePrimaryService(const ServiceDescription& service) = 0;

    virtual Galaxy galaxy() const = 0;

    virtual ServiceDescription service() const = 0;

    virtual void pulse() = 0;
    
    virtual GalaxyList getGalaxySnapshot() = 0;

    virtual ServiceList getServiceSnapshot(
        const Galaxy& galaxy) = 0;
};

class NullServerDirectory : public ServiceDirectoryInterface {
public:
    ~NullServerDirectory() {}

    bool registerService(
        const std::string& name, 
        const std::string& service_type, 
        const std::string& version, 
        const std::string& address, 
        uint16_t tcp_port, 
        uint16_t udp_port, 
        uint16_t ping) 
    {
        return false;
    }
    
    bool removeService(const ServiceDescription& service) {
        return false;
    }
    
    void updateServiceStatus(int32_t new_status) {}
    
    bool makePrimaryService(const ServiceDescription& service) {
        return false;
    }

    void pulse() {}
    
    GalaxyList getGalaxySnapshot() {
        GalaxyList galaxy_list;
        return galaxy_list;
    }

    ServiceList getServiceSnapshot(const Galaxy& galaxy) {
        ServiceList service_list;
        return service_list;
    }
};

}  // namespace service
}  // namespace swganh
