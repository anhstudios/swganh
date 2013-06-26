// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <turtle/mock.hpp>

#include "server.h"

namespace swganh{ 
namespace network {
  
MOCK_BASE_CLASS(MockServer, ServerInterface )
{
    MOCK_METHOD(StartListening, 1);
    MOCK_METHOD(StopListening, 0);
    MOCK_METHOD(SendTo, 2);
    MOCK_METHOD(HandleMessage, 2);    

    MOCK_METHOD(RemoveSession, 1);
    MOCK_METHOD(CreateSession, 1);
    MOCK_METHOD(GetSession, 1);

    MOCK_METHOD(max_receive_size, 0);
    MOCK_METHOD(listen_endpoint, 0);
};
    
}}  // namespace swganh::network
