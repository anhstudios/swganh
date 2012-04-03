// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_POSTURE_H_
#define SWGANH_MESSAGES_CONTROLLERS_POSTURE_H_

#include <cstdint>

#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class Posture : public ObjControllerMessage
    {
    public:
        explicit Posture(uint32_t controller_type = 0x0000001B)
            : ObjControllerMessage(controller_type, message_type())
        {}

        explicit Posture(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x00000131; }
        
        uint8_t posture_id;
        
        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(posture_id);
    		buffer.write<uint8_t>(1);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            posture_id = buffer.read<uint8_t>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_POSTURE_H_
