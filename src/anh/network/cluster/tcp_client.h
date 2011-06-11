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

#ifndef ANH_NETWORK_CLUSTER_TCP_CLIENT_H_
#define ANH_NETWORK_CLUSTER_TCP_CLIENT_H_

#include <anh/byte_buffer.h>

#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace anh {
namespace network {
namespace cluster {
using boost::asio::ip::tcp;

class tcp_client
{
public:
    tcp_client(boost::asio::io_service& service, const std::string& host, uint16_t port);

    /**
     * @brief Default Deconstructor.
     */
    ~tcp_client(void);

    /**
     * @brief Sends a message on the wire
     */
    void Send(anh::ByteBuffer& buffer);

    /**
     * @returns The amount of bytes received on the socket.
     */
    const uint64_t& bytes_recv() { return bytes_recv_; }

    /**
     * @returns The amount of bytes sent on the socket.
     */
    const uint64_t& bytes_sent() { return bytes_sent_; }

private:
    void handle_connect_(const boost::system::error_code& error, boost::asio::ip::tcp::endpoint endpoint);

    uint64_t	bytes_recv_;
    uint64_t	bytes_sent_;

    tcp::socket		                    socket_;
    tcp::endpoint		                current_remote_endpoint_;
    tcp::resolver                       resolver_;
    boost::array<char, 456>				recv_buffer_;
};
} // namespace cluster
} // namespace network
} // namespace anh

#endif // ANH_NETWORK_CLUSTER_TCP_CLIENT_H_