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

#ifndef ANH_SERVICE_SERVICE_H_
#define ANH_SERVICE_SERVICE_H_

#include <cstdint>
#include <string>

namespace anh {
namespace service {

class ServiceDescription {    
public:
    ServiceDescription();

    /*! This overloaded constructor is used when describing a service to register.
    *
    * \param type The type of the service.
    * \param version The version of the service.
    * \param address The address to communicate with the service.
    * \param tcp_port The tcp port to connect to the service on, default to 0 if not used.
    * \param udp_port The udp port to connect to the service on, default to 0 if not used.
    * \param status The current status of the service.
    * \param last_pulse The last time the service synced with the data store.
    */
    ServiceDescription(const std::string& name,
            const std::string& type,
            const std::string& version,
            const std::string& address,
            uint16_t tcp_port,
            uint16_t udp_port,
            uint16_t ping_port); 

    /*! This overloaded constructor is used when creating an instance from
    * the data store.
    *
    * \param id The id of the service in the data store.
    * \param galaxy_id The id of the galaxy this service belongs to.
    * \param type The type of the service.
    * \param version The version of the service.
    * \param address The address to communicate with the service.
    * \param tcp_port The tcp port to connect to the service on, default to 0 if not used.
    * \param udp_port The udp port to connect to the service on, default to 0 if not used.
    * \param status The current status of the service.
    * \param last_pulse The last time the service synced with the data store.
    */
    ServiceDescription(uint32_t id,
            uint32_t galaxy_id,
            const std::string& name,
            const std::string& type,
            const std::string& version,
            const std::string& address,
            uint16_t tcp_port,
            uint16_t udp_port,
            uint16_t ping_port);    

    /// Destructor
    ~ServiceDescription();    

    /// Copy constructor.
    ServiceDescription(const ServiceDescription& other);

    /// Move constructor.
    ServiceDescription(ServiceDescription&& other);
    
    /// Swap the contents of two HashStrings.
    void swap(ServiceDescription& other);

    /// Universal assignment operator.
    ServiceDescription& operator=(ServiceDescription other);

    /// Equals Operator.
    bool operator==(ServiceDescription other)
    {
        return this->id_ == other.id_;
    }
    /// Not Equals Operator.
    bool operator!=(ServiceDescription other)
    {
        return this->id_ != other.id_;
    }
        
    /*! Returns the id of the service in the data store.
    *
    * \return Returns the id of the service in the data store.
    */
    uint32_t id() const;
    void id(uint32_t id);

    /*! Returns The id of the galaxy this service belongs to.
    *
    * \return Returns The id of the galaxy this service belongs to.
    */
    uint32_t galaxy_id() const;

    /*! Returns The name of the service.
    *
    * \return Returns The name of the service.
    */
    const std::string& name() const;    

    /*! Returns The type of the service.
    *
    * \return Returns The type of the service.
    */
    const std::string& type() const;    
    
    /*! Returns the version of the service.
    *
    * \return Returns the version of the service.
    */
    const std::string& version() const;    
    
    /*! Returns the address to communicate with the service.
    *
    * \return Returns the address to communicate with the service.
    */
    const std::string& address() const;    
    
    /*! Returns the tcp port to connect to the service on, default to 0 if not used.
    *
    * \return Returns the tcp port to connect to the service on, default to 0 if not used.
    */
    uint16_t tcp_port() const;    
    
    /*! Returns the udp port to connect to the service on, default to 0 if not used.
    *
    * \return Returns the udp port to connect to the service on, default to 0 if not used.
    */
    uint16_t udp_port() const;    
    
    /*! Returns the ping port, default to 0 if not used.
    *
    * \return Returns the udp port, default to 0 if not used.
    */
    uint16_t ping_port() const;    
    
    /*! Returns the current status of the service.
    *
    * \return Returns -1 if the service is not operational otherwise it returns
    *   the number of connected clients.
    */
    int32_t status() const;    
    void status(int32_t new_status);
    
    /*! Returns the last time the service synced with the data store.
    *
    * \return Returns the last time the service synced with the data store.
    */
    const std::string& last_pulse() const;
    void last_pulse(std::string last_pulse);
    
private:
    friend class ServiceDirectory;
    
    uint32_t id_;
    uint32_t galaxy_id_;
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

}  // namespace service
}  // namespace anh

#endif  // ANH_SERVICE_SERVICE_H_
