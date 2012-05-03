// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_SIT_ON_OBJECT_H_
#define SWGANH_MESSAGES_CONTROLLERS_SIT_ON_OBJECT_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class SitOnObject : public ObjControllerMessage
    {
    public:
        explicit SitOnObject(uint32_t controller_type = 0x0000001B)
            : ObjControllerMessage(controller_type, message_type())
            , cell_id(0)
            , coord_x(0.0f)
            , coord_z(0.0f)
            , coord_y(0.0f)
        {}

        explicit SitOnObject(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x0000013B; }
        
        uint64_t cell_id;
        float coord_x;
        float coord_z;
        float coord_y;
        

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(cell_id);
            buffer.write(coord_x);
            buffer.write(coord_z);
            buffer.write(coord_y);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            cell_id = buffer.read<uint64_t>();
            coord_x = buffer.read<float>();
            coord_z = buffer.read<float>();
            coord_y = buffer.read<float>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_SIT_ON_OBJECT_H_
