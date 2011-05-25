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

#ifndef ANH_NETWORK_CLUSTER_SERVICE_H_
#define ANH_NETWORK_CLUSTER_SERVICE_H_

#include <anh/byte_buffer.h>
#include <anh/network/cluster/tcp_client.h>
#include <anh/network/cluster/tcp_host.h>
#include <anh/server_directory/server_directory_interface.h>
#include <anh/event_dispatcher/event_interface.h>

#include <map>
#include <list>
#include <stdint.h>

#include <tbb/pipeline.h>
#include <tbb/concurrent_vector.h>
#undef SendMessage;
namespace anh {
namespace network {
namespace cluster {

typedef std::map<std::shared_ptr<anh::server_directory::Process>, std::shared_ptr<tcp_client>>            ClusterMap;
typedef std::pair<std::shared_ptr<anh::server_directory::Process>, std::shared_ptr<tcp_client>>           ClusterPair;
/**
 * @brief Represent a Cluster Service
 */
class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(boost::asio::io_service& io_service, std::shared_ptr<anh::server_directory::ServerDirectoryInterface> directory);
    ~Service(void);

    /**
     * @brief Starts the Cluster Service.
     * 
     * @param port The port to listen for messages on.
     */
    void Start(uint16_t port);
    void Update(void);
    void Shutdown(void);
    /**
     * @brief Opens a TCP connection to the given host
     * 
     * @param process the server directory process to connect to.
     */
    void Connect(std::shared_ptr<anh::server_directory::Process> process);
    /**
     * @brief Sends a message to given the name of the service
     * 
     * @param name The service name to send a message to.
     * @param event_out the event that will be sent out.
     */
    void SendMessage(const std::string& name, std::shared_ptr<anh::event_dispatcher::EventInterface> event_out);
    /**
     * @brief Sends a message to given the name of the service
     * 
     * @param host The host address to send a message to.
     * @param port The port to the host.
     * @param buffer The ByteBuffer message to send
     */
    void SendMessage(const std::string& host, uint16_t port, anh::ByteBuffer& buffer);
    /**
     * @brief Checks to see if there is a connection to the given service
     * 
     * @param name the service name to check
     * @returns true if the service was found.
     */
    bool isConnected(const std::string& name);
    /**
     * @brief Checks to see if there is a connection to the given service
     * 
     * @param process the service process to check
     * @returns true if the service was found.
     */
    bool isConnected(std::shared_ptr<anh::server_directory::Process> process);
    /**
     * @brief Checks to see if there is a connection to the given address and port
     * 
     * @param host the address to send to
     * @param port the host port
     * @returns connection if found, else nullptr
     */
    std::shared_ptr<tcp_client> getConnection(const std::string& host, uint16_t port);
    /**
    * @brief Gets the named connection
    *
    * @param name the name of the connection to grab
    * @returns tcp_client if found, else nullptr
    */
    std::shared_ptr<tcp_client> getConnection(const std::string& name);

    std::shared_ptr<tcp_host>  host() { return tcp_host_; }
private:
    /**
     * @brief Called when the socket receives a message.
     *
     * This function will also attempt to find the session in the SessionManager,
     * if it fails to find a Session it will check if the incoming message is a
     * Session Request in which case, it will create a new Session.
     */
    void handle_accept_(std::shared_ptr<tcp_host> conn, const boost::system::error_code& error);

    tcp::resolver                                                       resolver_;
    anh::server_directory::ProcessList                                  proc_list_;
    boost::asio::io_service& 		                                    io_service_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor>                     acceptor_;
    std::shared_ptr<tcp_host>                                           tcp_host_;
    std::shared_ptr<anh::server_directory::ServerDirectoryInterface>    directory_;
    ClusterMap                                                          tcp_client_map_;
    
};

} // namespace cluster
} // namespace anh
} // namespace network

#endif // ANH_NETWORK_CLUSTER_SERVICE_H_