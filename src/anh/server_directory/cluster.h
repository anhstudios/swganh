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

#ifndef ANH_SERVER_DIRECTORY_CLUSTER_H_
#define ANH_SERVER_DIRECTORY_CLUSTER_H_

#include <cstdint>
#include <string>

namespace anh {
namespace server_directory {

class Cluster {
public:
    enum StatusType {
        OFFLINE = 0,
        LOADING = 1,
        ONLINE = 2,
        LOCKED = 3
    };
    
public:
    /*! This overloaded constructor is used when creating an instance from
    * the data store.
    *
    * \param id The id of the cluster in the data store.
    * \param primary_id The id of the primary process for this cluster.
    * \param name The name of the cluster.
    * \param status The current status of the cluster.
    * \param created_at The timestamp indicating the time the cluster was first created.
    * \param updated_at The last time this cluster was modified manually.
    */
    Cluster(uint32_t id, 
            uint32_t primary_id, 
            const std::string& name, 
            const std::string& version,
            Cluster::StatusType status, 
            const std::string& created_at, 
            const std::string& updated_at);

    /// Copy constructor.
    Cluster(const Cluster& other);

    /// Move constructor.
    Cluster(Cluster&& other);
    
    /// Swap the contents of two Clusters.
    void swap(Cluster& other);

    /// Universal assignment operator.
    Cluster& operator=(Cluster other);
        
    /*! Returns the id of the cluster in the data store.
    *
    * \returns Returns the id of the cluster in the data store.
    */
    uint32_t id() const;
        
    /*! Returns the id of the primary process for this cluster.
    *
    * \returns Returns the id of the primary process for this cluster.
    */
    uint32_t primary_id() const;
        
    /*! Returns the name of the cluster.
    *
    * \returns Returns the name of the cluster.
    */
    const std::string& name() const;    
        
    /*! Returns the version of the cluster.
    *
    * \returns Returns the version of the cluster.
    */
    const std::string& version() const;   
    
    /*! Returns the current status of the cluster.
    *
    * \returns Returns the current status of the cluster.
    */
    Cluster::StatusType status() const;    
    
    /*! Returns the timestamp indicating the time the cluster was first created.
    *
    * \returns Returns the timestamp indicating the time the cluster was first created.
    */
    const std::string& created_at() const;    
    
    /*! Returns the last time this cluster was modified manually.
    *
    * \returns Returns the last time this cluster was modified manually.
    */
    const std::string& updated_at() const;

private:
    friend class ServerDirectory;

    Cluster();

    void primary_id(uint32_t primary_id);
    
    uint32_t id_;
    uint32_t primary_id_;
    std::string name_;
    std::string version_;
    Cluster::StatusType status_;
    std::string created_at_;
    std::string updated_at_;
};

}  // namespace server_directory
}  // namespace anh

#endif  // ANH_SERVER_DIRECTORY_CLUSTER_H_
