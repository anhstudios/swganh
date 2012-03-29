
#ifndef SWGANH_MESSAGES_CONTROLLERS_DATA_TRANSFORM_H_
#define SWGANH_MESSAGES_CONTROLLERS_DATA_TRANSFORM_H_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {

    class DataTransform
    {
    public:
        static uint32_t message_type() { return 0x00000071; }
        uint32_t counter;
        glm::quat orientation;
        glm::vec3 position;
        float speed;

        void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(counter);
            buffer.write(orientation.x);
            buffer.write(orientation.y);
            buffer.write(orientation.z);
            buffer.write(orientation.w);
            buffer.write(position.x);
            buffer.write(position.y);
            buffer.write(position.z);
            buffer.write(speed);
        }

        void Deserialize(anh::ByteBuffer buffer)
        {
            counter = buffer.read<uint32_t>();
            orientation.x = buffer.read<float>();
            orientation.y = buffer.read<float>();
            orientation.z = buffer.read<float>();
            orientation.w = buffer.read<float>();
            position.x = buffer.read<float>();
            position.y = buffer.read<float>();
            position.z = buffer.read<float>();
            speed = buffer.read<float>();
        }
    };

}}}  // namespace swganh::messages::controllers


#endif  // SWGANH_MESSAGES_CONTROLLERS_DATA_TRANSFORM_H_
