/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
 
#ifndef ANH_NETWORK_CLUSTER_SOCKET_H_
#define ANH_NETWORK_CLUSTER_SOCKET_H_

#include <cstdint>
#include <array>
#include <string>
#include <boost/asio.hpp>

#include "anh/network/socket_interface.h"

namespace anh {
namespace network {
namespace cluster {
    
class Socket : public anh::network::SocketInterface<boost::asio::ip::tcp> {
public:
    Socket(boost::asio::io_service& service, const std::string& host, uint16_t port, NetworkCallback callback);
    
    ~Socket();
    
    /** Sends a message on the wire to the target endpoint.
     */
    void Send(const endpoint& remote_endpoint, anh::ByteBuffer message);
        
    /** Starts the socket's async reading tasks.
     */
    void StartRead();

    /**
     * @return The amount of bytes received on the socket.
     */
    uint64_t bytes_recv() const {
        return bytes_recv_;
    }
    
    /**
     * @return The amount of bytes sent on the socket.
     */
    uint64_t bytes_sent() const {
        return bytes_sent_;
    }

private:
    uint64_t bytes_recv_;
    uint64_t bytes_sent_;
    NetworkCallback callback_;

    boost::asio::ip::tcp::socket    socket_;
    boost::asio::ip::tcp::endpoint  current_remote_endpoint_;
    boost::asio::ip::tcp::resolver  resolver_;
    
    enum { max_length = 1024 };
    std::array<unsigned char, max_length>    recv_buffer_;
};
    
}}}  // namespace anh::network::cluster 
 
#endif  // ANH_NETWORK_CLUSTER_SOCKET_H_ 
