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

#ifndef ANH_SERVER_DIRECTORY_PROCESS_H_
#define ANH_SERVER_DIRECTORY_PROCESS_H_

#include <cstdint>
#include <string>

namespace anh {
namespace server_directory {

class Process {    
public:
    /*! This overloaded constructor is used when creating an instance from
    * the data store.
    *
    * \param id The id of the process in the data store.
    * \param cluster_id The id of the cluster this process belongs to.
    * \param type The type of the process.
    * \param version The version of the process.
    * \param address The address to communicate with the process.
    * \param tcp_port The tcp port to connect to the process on, default to 0 if not used.
    * \param udp_port The udp port to connect to the process on, default to 0 if not used.
    * \param status The current status of the process.
    * \param last_pulse The last time the process synced with the data store.
    */
    Process(uint32_t id,
            uint32_t cluster_id,
            const std::string& name,
            const std::string& type,
            const std::string& version,
            const std::string& address,
            uint16_t tcp_port,
            uint16_t udp_port,
            uint16_t ping_port);    

    /// Destructor
    ~Process();    

    /// Copy constructor.
    Process(const Process& other);

    /// Move constructor.
    Process(Process&& other);
    
    /// Swap the contents of two HashStrings.
    void swap(Process& other);

    /// Universal assignment operator.
    Process& operator=(Process other);
        
    /*! Returns the id of the process in the data store.
    *
    * \returns Returns the id of the process in the data store.
    */
    uint32_t id() const;

    /*! Returns The id of the cluster this process belongs to.
    *
    * \returns Returns The id of the cluster this process belongs to.
    */
    uint32_t cluster_id() const;

    /*! Returns The name of the process.
    *
    * \returns Returns The name of the process.
    */
    const std::string& name() const;    

    /*! Returns The type of the process.
    *
    * \returns Returns The type of the process.
    */
    const std::string& type() const;    
    
    /*! Returns the version of the process.
    *
    * \returns Returns the version of the process.
    */
    const std::string& version() const;    
    
    /*! Returns the address to communicate with the process.
    *
    * \returns Returns the address to communicate with the process.
    */
    const std::string& address() const;    
    
    /*! Returns the tcp port to connect to the process on, default to 0 if not used.
    *
    * \returns Returns the tcp port to connect to the process on, default to 0 if not used.
    */
    uint16_t tcp_port() const;    
    
    /*! Returns the udp port to connect to the process on, default to 0 if not used.
    *
    * \returns Returns the udp port to connect to the process on, default to 0 if not used.
    */
    uint16_t udp_port() const;    
    
    /*! Returns the ping port, default to 0 if not used.
    *
    * \returns Returns the udp port, default to 0 if not used.
    */
    uint16_t ping_port() const;    
    
    /*! Returns the current status of the process.
    *
    * \returns Returns -1 if the process is not operational otherwise it returns
    *   the number of connected clients.
    */
    int32_t status() const;    
    void status(int32_t new_status);
    
    /*! Returns the last time the process synced with the data store.
    *
    * \returns Returns the last time the process synced with the data store.
    */
    const std::string& last_pulse() const;
    void last_pulse(std::string last_pulse);
    
private:
    friend class ServerDirectory;

    Process();

    
    uint32_t id_;
    uint32_t cluster_id_;
    std::string name_;
    std::string type_;
    std::string version_;
    std::string address_;
    uint16_t tcp_port_;
    uint16_t udp_port_;
    uint16_t ping_port_;
    int32_t status_;
    std::string last_pulse_;
};

}  // namespace server_directory
}  // namespace anh

#endif  // ANH_SERVER_DIRECTORY_PROCESS_H_
