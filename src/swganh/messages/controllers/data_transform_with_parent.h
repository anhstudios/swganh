
#ifndef SWGANH_MESSAGES_CONTROLLERS_DATA_TRANSFORM_WITH_PARENT_H_
#define SWGANH_MESSAGES_CONTROLLERS_DATA_TRANSFORM_WITH_PARENT_H_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class DataTransformWithParent : public ObjControllerMessage
    {
    public:
        explicit DataTransformWithParent(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
        {}

        explicit DataTransformWithParent(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x000000F1; }
        uint32_t counter;
        uint64_t cell_id;
        glm::quat orientation;
        glm::vec3 position;
        float speed;

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(counter);
            buffer.write(cell_id);
            buffer.write(orientation.x);
            buffer.write(orientation.y);
            buffer.write(orientation.z);
            buffer.write(orientation.w);
            buffer.write(position.x);
            buffer.write(position.y);
            buffer.write(position.z);
            buffer.write(speed);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            counter = buffer.read<uint32_t>();
            cell_id = buffer.read<uint64_t>();
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


#endif  // SWGANH_MESSAGES_CONTROLLERS_DATA_TRANSFORM_WITH_PARENT_H_
