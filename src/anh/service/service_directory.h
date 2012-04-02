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

#ifndef ANH_SERVICE_SERVICE_DIRECTORY_H_
#define ANH_SERVICE_SERVICE_DIRECTORY_H_

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include "anh/service/galaxy.h"
#include "anh/service/service_description.h"
#include "anh/service/service_directory_interface.h"

// Forward Declare
namespace anh { class EventDispatcher; }

namespace anh {
namespace service {

class DatastoreInterface;

class InvalidGalaxyError : public std::runtime_error {
public:
    InvalidGalaxyError(const std::string& message = "") 
        : std::runtime_error(message) {}
};

class InvalidServiceError : public std::runtime_error {
public:
    InvalidServiceError(const std::string& message = "") 
        : std::runtime_error(message) {}
};

/*! \brief ServiceDirectory is a utility class intended to assist servicees in
* registering themselves and participating in a galaxyed environment.
*/
class ServiceDirectory : public ServiceDirectoryInterface{
public:
    ServiceDirectory(std::shared_ptr<DatastoreInterface> datastore, 
        anh::EventDispatcher* event_dispatcher);
    ServiceDirectory(std::shared_ptr<DatastoreInterface> datastore, 
        anh::EventDispatcher* event_dispatcher, const std::string& galaxy_name, 
        const std::string& version = "", bool create_galaxy = false);

    Galaxy galaxy() const;
    ServiceDescription service() const;
        
    void joinGalaxy(const std::string& galaxy_name, const std::string& version = "", bool create_galaxy = false);
    
    void updateGalaxyStatus();

    bool registerService(ServiceDescription& service);
    bool removeService(const ServiceDescription& service);
    void updateService(const ServiceDescription& service);
    void updateServiceStatus(int32_t new_status);
    
    bool makePrimaryService(const ServiceDescription& service);

    void pulse();
    
    GalaxyList getGalaxySnapshot();
    ServiceList getServiceSnapshot(const Galaxy& galaxy);

private:
    std::string getGalaxyTimestamp_();

    std::shared_ptr<DatastoreInterface> datastore_;
    Galaxy active_galaxy_;
    ServiceDescription active_service_;

    std::mutex mutex_;

    anh::EventDispatcher* event_dispatcher_;
};

}  // namespace service
}  // namespace anh

#endif  // ANH_SERVICE_SERVICE_DIRECTORY_H_
