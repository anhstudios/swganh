/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef ANH_NETWORK_CLUSTER_SERVICE_INTERFACE_H_
#define ANH_NETWORK_CLUSTER_SERVICE_INTERFACE_H_

#include <anh/byte_buffer.h>
#include <anh/event_dispatcher/event_interface.h>
#include <anh/event_dispatcher/event_dispatcher_interface.h>
#include <anh/server_directory/server_directory_interface.h>
#include <anh/network/cluster/tcp_client.h>
#include <anh/network/cluster/tcp_host.h>
#include <anh/network/cluster/tcp_message.h>

namespace anh {
namespace network {
namespace cluster {
class ClusterServiceInterface {
public:

    /**
     * @brief Starts the Cluster Service.
     *
     * @param port The port to listen for messages on.
     */
    virtual void Start(std::shared_ptr<tcp_host> host = nullptr) = 0;

    /**
    *  Runs the pipeline each tick.
    */
    virtual void Update(void) = 0;
    /**
    * Prepares the Service for destruction
    */
    virtual void Shutdown(void) = 0;
    /**
     * @brief Opens a TCP connection to the given host
     *
     * @param process the server directory process to connect to.
     */
    virtual void Connect(std::shared_ptr<anh::server_directory::Process> process) = 0;
    /**
     * @brief Opens a TCP connection to the tcp client connection
     *
     * @param tcp client to add to the map and connect.
     * @param process to add to the map
     */
    virtual void Connect(std::shared_ptr<tcp_client> client, std::shared_ptr<anh::server_directory::Process> process) = 0;
    /**
    * @brief Closes the TCP Connection
    * @param process the service directory process to disconnect
    */
    virtual void Disconnect(std::shared_ptr<anh::server_directory::Process> process) = 0;
    /**
     * @brief Sends a message to given the name of the service
     *
     * @param name The service name to send a message to.
     * @param event_out the event that will be sent out.
     * @param (optional) DestinationType the destination that the message will be sent to.
     */
    virtual void sendMessage(const std::string& name, std::shared_ptr<anh::event_dispatcher::EventInterface> event_out
        , DestinationType dest = SINGLE) = 0;
    /**
     * @brief Sends a message to given the name of the service
     *
     * @param host The host address to send a message to.
     * @param port The port to the host.
     * @param buffer The ByteBuffer message to send
     * @param DestinationType the destination that the message will be sent to
     * defaults to SINGLE.
     */
    virtual void sendMessage(const std::string& host, uint16_t port, anh::ByteBuffer& buffer
        , DestinationType dest = SINGLE) = 0;

    /**
     * @brief Checks to see if there is a connection to the given service
     *
     * @param name the service name to check
     * @returns true if the service was found.
     */
    virtual bool isConnected(const std::string& name) = 0;
    /**
     * @brief Checks to see if there is a connection to the given service
     *
     * @param process the service process to check
     * @returns true if the service was found.
     */
    virtual bool isConnected(std::shared_ptr<anh::server_directory::Process> process) = 0;
    /**
     * @brief Checks to see if there is a connection to the given address and port
     *
     * @param host the address to send to
     * @param port the host port
     * @returns connection if found, else nullptr
     */
    virtual std::shared_ptr<tcp_client> getConnection(const std::string& host, uint16_t port) = 0;
    /**
    * @brief Gets the named connection
    *
    * @param name the name of the connection to grab
    * @returns tcp_client if found, else nullptr
    */
    virtual std::shared_ptr<tcp_client> getConnection(const std::string& name) = 0;

    /*
    * Gets the TCP host object
    */
    virtual std::shared_ptr<tcp_host>  host() = 0;
};
} // cluster
} // network
} // anh

#endif //ANH_NETWORK_CLUSTER_SERVICE_INTERFACE_H_