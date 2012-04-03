// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_SCENE_DESTROY_OBJECT_H_
#define SWGANH_MESSAGES_SCENE_DESTROY_OBJECT_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct SceneDestroyObject : public BaseSwgMessage<SceneDestroyObject> 
    {
        static uint16_t Opcount() { return 3; }
        static uint32_t Opcode() { return 0x4D45D504; }
        
        uint64_t object_id;

        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(object_id);
            buffer.write<uint8_t>(0);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
            object_id = buffer.read<uint64_t>();
            buffer.read<uint8_t>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_SCENE_DESTROY_OBJECT_H_
