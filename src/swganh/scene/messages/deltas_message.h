
#ifndef SWGANH_SCENE_MESSAGES_DELTAS_MESSAGE_H_
#define SWGANH_SCENE_MESSAGES_DELTAS_MESSAGE_H_

#include <cstdint>

#include "anh/byte_buffer.h"

#include "swganh/scene/messages/base_baselines_message.h"

namespace swganh {
namespace scene {
namespace messages {
    
class DeltasMessage : public BaseBaselineMessage<DeltasMessage> {
public:
    static const uint16_t opcount = 5;
    static const uint32_t opcode = 0x12862153;
};
    
}}}  // namespace swganh::scene::messages

#endif  // SWGANH_SCENE_MESSAGES_DELTAS_MESSAGE_H_
