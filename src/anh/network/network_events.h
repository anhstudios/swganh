
#ifndef ANH_NETWORK_NETWORK_EVENTS_H_
#define ANH_NETWORK_NETWORK_EVENTS_H_

#include <cstdint>
#include "anh/byte_buffer.h"

namespace anh {
namespace network {

struct RemoteMessage {
    RemoteMessage() {}
    RemoteMessage(uint32_t source_id, anh::ByteBuffer data) 
        : source_id(source_id)
        , data(std::move(data))
    {}

    virtual ~RemoteMessage() {}
    
    uint32_t source_id;
    anh::ByteBuffer data;
    
    void serialize(anh::ByteBuffer&) const {}
    void deserialize(anh::ByteBuffer) {}
};

}}  // namespace anh::network

#endif  // ANH_NETWORK_NETWORK_EVENTS_H_
