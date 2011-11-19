#ifndef SWGANH_MESSAGES_HEART_BEAT_H_
#define SWGANH_MESSAGES_HEART_BEAT_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {
    
struct HeartBeat : public swganh::messages::BaseSwgMessage<HeartBeat> {
    static const uint16_t opcount() { return 1; }
    static const uint32_t opcode() { return 0xA16CF9AF; }
    
    void onSerialize(anh::ByteBuffer& buffer) const {}

    void onDeserialize(anh::ByteBuffer buffer) {}
};

}}  // namespace swganh::messages

#endif // SWGANH_MESSAGES_HEART_BEAT_H_
