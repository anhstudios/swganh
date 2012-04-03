// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_ANIMATE_H_
#define SWGANH_MESSAGES_CONTROLLERS_ANIMATE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class Animate : public ObjControllerMessage
    {
    public:
        explicit Animate(uint32_t controller_type = 0x0000001B)
            : ObjControllerMessage(controller_type, message_type())
        {}

        explicit Animate(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        Animate(const std::string& animation_, uint64_t object_id_)
            : animation(animation_)
            , object_id(object_id_)
        {}

        static uint32_t message_type() { return 0x000000F2; }
        
        uint64_t object_id;
        std::string animation;

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(object_id);
            buffer.write<uint32_t>(0);
            buffer.write(animation);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            object_id = buffer.read<uint64_t>();
            buffer.read<uint32_t>();
            animation = buffer.read<std::string>();
        }
    };

}}}  // namespace swganh::messages::controllers


#endif  // SWGANH_MESSAGES_CONTROLLERS_SHOW_FLY_TEXT_H_
