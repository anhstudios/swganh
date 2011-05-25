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

#include <map>
#include <list>
#include <stdint.h>

#include <tbb/pipeline.h>
#include <tbb/concurrent_vector.h>

namespace anh {
namespace network {
namespace cluster {

typedef std::pair<std::shared_ptr<tcp_client>, std::shared_ptr<anh::server_directory::Process>> ConnectionPair;
typedef std::map<std::shared_ptr<tcp_client>, std::shared_ptr<anh::server_directory::Process>> ConnectionMap;
typedef std::map<std::string, std::shared_ptr<tcp_client>>            ClientMap;
typedef std::pair<std::string, std::shared_ptr<tcp_client>>           ClientPair;
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
     * @parama port The port to listen for messages on.
     */
    void Start(uint16_t port);
    void Update(void);
    void Shutdown(void);

    void Connect(const std::string& host, const std::string& port);
    void SendMessage(const std::string& host_and_port, anh::ByteBuffer& buffer);

    std::shared_ptr<tcp_host>  connection() { return connection_; }
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
    std::shared_ptr<tcp_host>                                           connection_;
    anh::server_directory::ProcessList                                  proc_list_;
    boost::asio::io_service& 		                                    io_service_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor>                     acceptor_;
    std::shared_ptr<tcp_host>                                           tcp_host_;
    std::shared_ptr<anh::server_directory::ServerDirectoryInterface>    directory_;
    ClientMap                                                           tcp_client_map_;
    
};

} // namespace cluster
} // namespace anh
} // namespace network

#endif // ANH_NETWORK_CLUSTER_SERVICE_H_