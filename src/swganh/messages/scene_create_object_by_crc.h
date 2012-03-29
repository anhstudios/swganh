// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_SCENE_CREATE_OBJECT_BY_CRC_H_
#define SWGANH_MESSAGES_SCENE_CREATE_OBJECT_BY_CRC_H_

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct SceneCreateObjectByCrc : public BaseSwgMessage<SceneCreateObjectByCrc>
    {
        static uint16_t Opcount() { return 5; }
        static uint32_t Opcode() { return 0xFE89DDEA; }
        
        uint64_t object_id;
        glm::quat orientation;
        glm::vec3 position;
        uint32_t object_crc;
        uint8_t byte_flag;
        
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
        	buffer.write(object_id);
            buffer.write(orientation.x);
            buffer.write(orientation.y);
            buffer.write(orientation.z);
            buffer.write(orientation.w);
            buffer.write(position.x);
            buffer.write(position.y);
            buffer.write(position.z);
            buffer.write(object_crc);
            buffer.write(byte_flag);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
        	object_id = buffer.read<uint64_t>();
            orientation.x = buffer.read<float>();
            orientation.y = buffer.read<float>();
            orientation.z = buffer.read<float>();
            orientation.w = buffer.read<float>();
            position.x = buffer.read<float>();
            position.y = buffer.read<float>();
            position.z = buffer.read<float>();
            object_crc = buffer.read<uint32_t>();
            byte_flag = buffer.read<uint8_t>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_SCENE_CREATE_OBJECT_BY_CRC_H_
