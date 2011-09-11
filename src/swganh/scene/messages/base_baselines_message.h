
#ifndef SWGANH_SCENE_MESSAGES_BASE_BASELINES_MESSAGE_H_
#define SWGANH_SCENE_MESSAGES_BASE_BASELINES_MESSAGE_H_

#include <cstdint>

#include "anh/byte_buffer.h"

namespace swganh {
namespace scene {
namespace messages {
    
struct BaseBaselineMessage {    
    uint64_t object_id;
    uint32_t object_type;
    uint8_t view_type;    
    anh::ByteBuffer data;

    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(object_id);
        buffer.write(object_type);
        buffer.write(view_type);

        buffer.write(data.size());
        buffer.write(data.data(), data.size());        
    }

    void onDeserialize(anh::ByteBuffer buffer) {        
        object_id = buffer.read<uint32_t>();
        object_type = buffer.read<uint32_t>();  
        view_type = buffer.read<uint32_t>();

        uint32_t data_size = buffer.read<uint32_t>();
        data = anh::ByteBuffer(buffer.data() + buffer.read_position(), data_size);
    }
};
    
}}}  // namespace swganh::scene::messages

#endif  // SWGANH_SCENE_MESSAGES_BASE_BASELINES_MESSAGE_H_
