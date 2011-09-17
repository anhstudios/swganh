
#ifndef SWGANH_SCENE_MESSAGES_BASELINES_MESSAGE_H_
#define SWGANH_SCENE_MESSAGES_BASELINES_MESSAGE_H_

#include <cstdint>

#include "anh/byte_buffer.h"

#include "swganh/scene/messages/base_baselines_message.h"

namespace swganh {
namespace scene {
namespace messages {
    
struct BaselinesMessage : public BaseBaselineMessage<BaselinesMessage> {
    static const uint16_t opcount = 5;
    static const uint32_t opcode = 0x68A75F0C;
};
    
}}}  // namespace swganh::scene::messages

#endif  // SWGANH_SCENE_MESSAGES_BASELINES_MESSAGE_H_
