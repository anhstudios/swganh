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

#ifndef ANH_SERVER_DIRECTORY_SERVER_DIRECTORY_H_
#define ANH_SERVER_DIRECTORY_SERVER_DIRECTORY_H_

#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "anh/server_directory/galaxy.h"
#include "anh/server_directory/service.h"
#include "anh/server_directory/server_directory_interface.h"

// Forward Declare
namespace anh { namespace event_dispatcher { class EventDispatcherInterface; }  }

namespace anh {
namespace server_directory {

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

/*! \brief ServerDirectory is a utility class intended to assist servicees in
* registering themselves and participating in a galaxyed environment.
*/
class ServerDirectory : public ServerDirectoryInterface{
public:
    explicit ServerDirectory(std::shared_ptr<DatastoreInterface> datastore, 
        std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher);
    ServerDirectory(std::shared_ptr<DatastoreInterface> datastore, 
        std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher
        , const std::string& galaxy_name, const std::string& version = "", bool create_galaxy = false);

    std::shared_ptr<Galaxy> galaxy() const;
    std::shared_ptr<Service> service() const;
        
    void joinGalaxy(const std::string& galaxy_name, const std::string& version = "", bool create_galaxy = false);

    bool registerService(const std::string& name, const std::string& service_type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping);
    bool removeService(std::shared_ptr<Service>& service);
    void updateServiceStatus(std::shared_ptr<Service>& service, int32_t new_status);
    
    bool makePrimaryService(std::shared_ptr<Service> service);

    void pulse();

    GalaxyList getGalaxySnapshot() const;
    ServiceList getServiceSnapshot(std::shared_ptr<Galaxy> galaxy) const;

private:
    std::shared_ptr<DatastoreInterface> datastore_;
    std::shared_ptr<Galaxy> active_galaxy_;
    std::shared_ptr<Service> active_service_;

    std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface>    event_dispatcher_;
};

}  // namespace server_directory
}  // namespace anh

#endif  // ANH_SERVER_DIRECTORY_SERVER_DIRECTORY_H_
