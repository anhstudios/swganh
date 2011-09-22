
#ifndef SWGANH_NETWORK_REMOTE_CLIENT_H_
#define SWGANH_NETWORK_REMOTE_CLIENT_H_

#include "anh/byte_buffer.h"

namespace swganh {
namespace network {
    
    class RemoteClient
    {
    public:
        void Send(const anh::ByteBuffer& message);
    };
    
}}  // namespace swganh::network

#endif  // SWGANH_NETWORK_REMOTE_CLIENT_H_
