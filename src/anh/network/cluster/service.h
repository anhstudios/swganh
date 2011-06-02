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
#include <anh/network/cluster/tcp_message.h>
#include <anh/server_directory/server_directory_interface.h>
#include <anh/event_dispatcher/event_interface.h>
//Filters
#include <anh/network/cluster/send_packet_filter.h>
#include <anh/network/cluster/outgoing_start_filter.h>
#include <anh/network/cluster/packet_event_filter.h>
#include <anh/network/cluster/receive_packet_filter.h>

#include <boost/thread.hpp>

#include <map>
#include <stdint.h>

#include <tbb/pipeline.h>

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
    Service(boost::asio::io_service& io_service, std::shared_ptr<anh::server_directory::ServerDirectoryInterface> directory, uint16_t port);
    ~Service(void);

    /**
     * @brief Starts the Cluster Service.
     * 
     * @param port The port to listen for messages on.
     */
    void Start();
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
    void sendMessage(const std::string& name, std::shared_ptr<anh::event_dispatcher::EventInterface> event_out
        , Destination dest = SINGLE);
    /**
     * @brief Sends a message to given the name of the service
     * 
     * @param host The host address to send a message to.
     * @param port The port to the host.
     * @param buffer The ByteBuffer message to send
     */
    void sendMessage(const std::string& host, uint16_t port, anh::ByteBuffer& buffer
        , Destination dest = SINGLE);
    
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

    // Friend Filters. Filters are considered extensions of the
    // Service class as the operations performed in them are specific
    // to the TCP Cluster Service
    friend class SendPacketFilter;
    friend class OutgoingStartFilter;
    friend class ReceivePacketFilter;

private:

    void OnTCPHostReceive_(std::shared_ptr<anh::ByteBuffer> buffer);

    void handle_accept_(std::shared_ptr<tcp_host> conn, const boost::system::error_code& error);
    /**
     * @brief Sends a message to all services
     * 
     * @param buffer The ByteBuffer message to send
     */
    void sendMessageToAll_(anh::ByteBuffer& buffer);
    /**
     * @brief Sends a message to all types of the service
     * 
     * @param type the type of service to send to
     * @param buffer The ByteBuffer message to send
     */
    void sendMessageByType_(const std::string& type, anh::ByteBuffer& buffer);
    
    // Pipelines
    tbb::pipeline			incoming_pipeline_;
    tbb::pipeline			outgoing_pipeline_;
    // outgoing pipeline filters
    OutgoingStartFilter     outgoing_start_filter_;
    SendPacketFilter        send_packet_filter_;
    ReceivePacketFilter     receive_packet_filter_;
    // message lists
    std::list<TCPMessage*>  incoming_messages_;
    std::list<TCPMessage*>  outgoing_messages_;
    // asio needs
    tcp::resolver                                       resolver_;
    anh::server_directory::ProcessList                  proc_list_;
    boost::asio::io_service& 		                    io_service_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor>     acceptor_;
    // boost thread
    boost::thread                                       service_thread_;

    std::shared_ptr<tcp_host>                                           tcp_host_;
    uint16_t                                                            port_;
    std::shared_ptr<anh::server_directory::ServerDirectoryInterface>    directory_;
    ClusterMap                                                          tcp_client_map_;
    
};

} // namespace cluster
} // namespace anh
} // namespace network

#endif // ANH_NETWORK_CLUSTER_SERVICE_H_