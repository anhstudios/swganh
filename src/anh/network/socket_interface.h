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
 
#ifndef ANH_NETWORK_SOCKET_INTERFACE_H_
#define ANH_NETWORK_SOCKET_INTERFACE_H_

#include <cstdint>
#include <functional>
#include <memory>
#include "anh/byte_buffer.h"

namespace anh {
namespace network {
    
/** A common socket interface that all protocols in the anh library implement.
 */
template<typename T>
class SocketInterface {
public:
    // Create a typedef here so typename isn't used everywhere that T::endpoint is required.
    typedef typename T::endpoint endpoint;

    // Define a common callback structure to be used by concrete SocketInterface implementations.
    typedef std::function<void(endpoint, std::shared_ptr<anh::ByteBuffer>)> NetworkCallback;

    virtual ~SocketInterface() {}
    
    /** Sends a message on the wire to the target endpoint.
     */
    virtual void Send(const endpoint& remote_endpoint, anh::ByteBuffer message) = 0;
        
    /** Starts the socket's async reading tasks.
     */
    virtual void StartRead() = 0;

    /**
     * @return The amount of bytes received on the socket.
     */
    virtual uint64_t bytes_recv() const = 0;
    
    /**
     * @return The amount of bytes sent on the socket.
     */
    virtual uint64_t bytes_sent() const = 0;
};
    
}}  // namespace anh::network
 
#endif  // ANH_NETWORK_SOCKET_INTERFACE_H_
