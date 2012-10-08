// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_NETWORK_SOE_MOCK_SERVER_H_
#define ANH_NETWORK_SOE_MOCK_SERVER_H_

#include <turtle/mock.hpp>

#include "swganh/network/soe/server.h"

namespace swganh{

class ByteBuffer;
 
namespace network {
namespace soe {
    
class Session;
  
MOCK_BASE_CLASS(MockServer, ServerInterface )
{
    MOCK_METHOD(Startup, 1);
    MOCK_METHOD(Shutdown, 0);
    MOCK_METHOD(SendTo, 2);
    MOCK_METHOD(HandleMessage, 2);    

    MOCK_METHOD(RemoveSession, 1);
    MOCK_METHOD(CreateSession, 1);
    MOCK_METHOD(GetSession, 1);

    MOCK_METHOD(socket, 0);
    MOCK_METHOD(max_receive_size, 0);
};
    
}}}  // namespace swganh::network::soe

#endif  // ANH_NETWORK_SOE_MOCK_SERVER_H_
