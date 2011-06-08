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

#ifndef ANH_NETWORK_CLUSTER_TCP_MESSAGE_H
#define ANH_NETWORK_CLUSTER_TCP_MESSAGE_H

#include <boost/asio.hpp>

// FORWARD DECLARATIONS
namespace anh { class ByteBuffer; }

namespace anh {
namespace network {
namespace cluster {

// FORWARD DECLARATIONS
class tcp_client;

enum DestinationType {
    SINGLE = 1,
    TYPE  = 2,
    ALL = 3
};
/**
 * @brief a basic TCP Message to use on the queue
 */
class TCPMessage
{
public:
    TCPMessage(std::shared_ptr<tcp_client> client, std::shared_ptr<anh::ByteBuffer> message
        , DestinationType destination = SINGLE, const std::string& type = "none" )
        : client_(client)
        , message_(message)
        , destination_(destination)
        , type_(type)
    {
    }

    ~TCPMessage(void)
    {
    }
    DestinationType& destination() { return destination_; }
    std::shared_ptr<tcp_client> client() { return client_; }
    std::shared_ptr<anh::ByteBuffer> message() { return message_; }
    std::string& type() { return type_; }
private:
    DestinationType                         destination_;
    std::shared_ptr<tcp_client>			client_;
    std::shared_ptr<anh::ByteBuffer>	message_;
    std::string                         type_;
};

} // namespace cluster
} // namespace network
} // namespacce anh

#endif // ANH_NETWORK_CLUSTER_TCP_MESSAGE_H