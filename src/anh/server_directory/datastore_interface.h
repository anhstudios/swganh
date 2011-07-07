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

#ifndef ANH_SERVER_DIRECTORY_DATASTORE_INTERFACE_H_
#define ANH_SERVER_DIRECTORY_DATASTORE_INTERFACE_H_

#include <cstdint>

#include <list>
#include <memory>
#include <string>

namespace anh {
namespace server_directory {

class Cluster;
class Process;

class DatastoreInterface {
public:
    virtual ~DatastoreInterface() {}

    virtual std::shared_ptr<Cluster> findClusterByName(const std::string& name) const = 0;
    virtual std::shared_ptr<Cluster> createCluster(const std::string& name, const std::string& version) const = 0;
    virtual std::shared_ptr<Process> createProcess(std::shared_ptr<Cluster> cluster, const std::string& name, const std::string& type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping_port) const = 0;

    virtual std::string getClusterTimestamp(std::shared_ptr<Cluster> cluster) const = 0;

    virtual void saveProcess(std::shared_ptr<Process> process) const = 0;

    virtual std::shared_ptr<Cluster> findClusterById(uint32_t id) const = 0;

    virtual bool deleteProcessById(uint32_t id) const = 0;

    virtual std::list<Cluster> getClusterList() const = 0;
    virtual std::list<Process> getProcessList(uint32_t cluster_id) const = 0;
    virtual std::string prepareTimestampForStorage(const std::string& timestamp) const = 0;
};

}  // namespace server_directory
}  // namespace anh

#endif  // ANH_SERVER_DIRECTORY_DATASTORE_INTERFACE_H_
