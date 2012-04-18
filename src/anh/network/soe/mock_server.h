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

#ifndef ANH_NETWORK_SOE_MOCK_SERVER_H_
#define ANH_NETWORK_SOE_MOCK_SERVER_H_

#include <turtle/mock.hpp>

#include "anh/network/soe/server.h"

namespace anh{

class ByteBuffer;
 
namespace network {
namespace soe {
    
class Session;
  
MOCK_BASE_CLASS(MockServer, ServerInterface )
{
    MOCK_METHOD(Start, 1);
    MOCK_METHOD(Shutdown, 0);
    MOCK_METHOD(SendTo, 2);
    MOCK_METHOD(HandleMessage, 2);    

    MOCK_METHOD(RemoveSession, 1);
    MOCK_METHOD(CreateSession, 1);
    MOCK_METHOD(GetSession, 1);

    MOCK_METHOD(socket, 0);
    MOCK_METHOD(max_receive_size, 0);
};
    
}}}  // namespace anh::network::soe

#endif  // ANH_NETWORK_SOE_MOCK_SERVER_H_
