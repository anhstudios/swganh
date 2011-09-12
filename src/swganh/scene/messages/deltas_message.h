
#ifndef SWGANH_SCENE_MESSAGES_DELTAS_MESSAGE_H_
#define SWGANH_SCENE_MESSAGES_DELTAS_MESSAGE_H_

#include <cstdint>

#include "anh/byte_buffer.h"

#include "swganh/base/swg_message.h"
#include "swganh/scene/messages/base_baselines_message.h"

namespace swganh {
namespace scene {
namespace messages {
    
class DeltasMessage : public BaseBaselineMessage, public swganh::base::SwgMessage<DeltasMessage> {
public:
    static const uint16_t opcount = 5;
    static const uint32_t opcode = 0x12862153;

    void onSerialize(anh::ByteBuffer& buffer) const {
        BaseBaselineMessage::onSerialize(buffer);
    }

    void onDeserialize(anh::ByteBuffer buffer) {
        BaseBaselineMessage::onDeserialize(buffer);
    }
};
    
}}}  // namespace swganh::scene::messages

#endif  // SWGANH_SCENE_MESSAGES_DELTAS_MESSAGE_H_
