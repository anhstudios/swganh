#ifndef SWGANH_CONNECTION_MESSAGES_HEART_BEAT_H_
#define SWGANH_CONNECTION_MESSAGES_HEART_BEAT_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/base/swg_message.h"

namespace swganh {
namespace connection {
namespace messages {
    
struct HeartBeat : public swganh::base::SwgMessage<HeartBeat> {
    static const uint16_t opcount() { return 1; }
    static const uint32_t opcode() { return 0xA16CF9AF; }
    
    void onSerialize(anh::ByteBuffer& buffer) const {}

    void onDeserialize(anh::ByteBuffer buffer) {}
};

}}}  // namespace swganh::connection::messages

#endif // SWGANH_CONNECTION_MESSAGES_HEART_BEAT_H_
