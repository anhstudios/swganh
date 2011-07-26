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

#ifndef ANH_SERVICE_DATASTORE_INTERFACE_H_
#define ANH_SERVICE_DATASTORE_INTERFACE_H_

#include <cstdint>

#include <list>
#include <memory>
#include <string>

namespace anh {
namespace service {

class Galaxy;
class ServiceDescription;

class DatastoreInterface {
public:
    virtual ~DatastoreInterface() {}

    virtual std::shared_ptr<Galaxy> findGalaxyByName(const std::string& name) const = 0;
    virtual std::shared_ptr<Galaxy> createGalaxy(const std::string& name, const std::string& version) const = 0;
    virtual std::shared_ptr<ServiceDescription> createService(std::shared_ptr<Galaxy> galaxy, const std::string& name, const std::string& type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping_port) const = 0;

    virtual std::string getGalaxyTimestamp(std::shared_ptr<Galaxy> galaxy) const = 0;

    virtual void saveService(std::shared_ptr<ServiceDescription> service) const = 0;

    virtual std::shared_ptr<Galaxy> findGalaxyById(uint32_t id) const = 0;

    virtual bool deleteServiceById(uint32_t id) const = 0;

    virtual std::list<Galaxy> getGalaxyList() const = 0;
    virtual std::list<ServiceDescription> getServiceList(uint32_t galaxy_id) const = 0;
    virtual std::string prepareTimestampForStorage(const std::string& timestamp) const = 0;
};

}  // namespace service
}  // namespace anh

#endif  // ANH_SERVICE_DATASTORE_INTERFACE_H_
