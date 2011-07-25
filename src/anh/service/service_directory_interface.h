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

#ifndef ANH_SERVICE_SERVICE_DIRECTORY_INTERFACE_H_
#define ANH_SERVICE_SERVICE_DIRECTORY_INTERFACE_H_

#include <cstdint>

#include <list>
#include <memory>
#include <string>

#include "anh/service/galaxy.h"
#include "anh/service/service.h"

namespace anh {
namespace service {

typedef std::list<Galaxy> GalaxyList;
typedef std::list<ServiceDescription> ServiceList;

/// Simple interface
class ServiceDirectoryInterface {
public:
    virtual ~ServiceDirectoryInterface() {}

    virtual void joinGalaxy(const std::string& galaxy_name, const std::string& version = "", bool create_galaxy = false) = 0;

    virtual bool registerService(
        const std::string& name, 
        const std::string& service_type, 
        const std::string& version, 
        const std::string& address, 
        uint16_t tcp_port, 
        uint16_t udp_port, 
        uint16_t ping) = 0;
    
    virtual bool removeService(std::shared_ptr<ServiceDescription>& service) = 0;
    
    virtual void updateServiceStatus(
        std::shared_ptr<ServiceDescription>& service, 
        int32_t new_status) = 0;
    
    virtual bool makePrimaryService(std::shared_ptr<ServiceDescription> service) = 0;

    virtual std::shared_ptr<Galaxy> galaxy() const = 0;

    virtual std::shared_ptr<ServiceDescription> service() const = 0;

    virtual void pulse() = 0;
    
    virtual GalaxyList getGalaxySnapshot() const = 0;

    virtual ServiceList getServiceSnapshot(
        std::shared_ptr<Galaxy> galaxy) const = 0;
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
    
    bool removeService(std::shared_ptr<ServiceDescription>& service) {
        return false;
    }
    
    void updateServiceStatus(
        std::shared_ptr<ServiceDescription>& service, 
        int32_t new_status) {}
    
    bool makePrimaryService(std::shared_ptr<ServiceDescription> service) {
        return false;
    }

    void pulse() {}
    
    GalaxyList getGalaxySnapshot() const {
        GalaxyList galaxy_list;
        return galaxy_list;
    }

    ServiceList getServiceSnapshot(std::shared_ptr<Galaxy> galaxy) const {
        ServiceList service_list;
        return service_list;
    }
};

}  // namespace service
}  // namespace anh

#endif  // ANH_SERVICE_SERVICE_DIRECTORY_INTERFACE_H_
