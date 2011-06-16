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

#ifndef ANH_NETWORK_MOCK_SOCKET_H_
#define ANH_NETWORK_MOCK_SOCKET_H_

#include <gmock/gmock.h>
#include "anh/network/socket_interface.h"

namespace anh{
namespace network {
  
template<typename T>  
class MockSocket : public SocketInterface<T> {
public:
    MOCK_METHOD2_T(Send, void(const endpoint& remote_endpoint, anh::ByteBuffer message));
    MOCK_METHOD0_T(StartRead, void());
    MOCK_CONST_METHOD0_T(bytes_recv, uint64_t());
    MOCK_CONST_METHOD0_T(bytes_sent, uint64_t());
};
    
}}  // namespace anh::network

#endif  // ANH_NETWORK_MOCK_SOCKET_H_