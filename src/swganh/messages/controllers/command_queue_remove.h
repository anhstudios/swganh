// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_REMOVE_H_
#define SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_REMOVE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class CommandQueueRemove : public ObjControllerMessage
    {
    public:
        explicit CommandQueueRemove(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
        {}

        explicit CommandQueueRemove(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x00000117; }
        
        uint32_t action_counter;
        float timer;
        uint32_t error;
        uint32_t action;

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(action_counter);
            buffer.write(timer);
            buffer.write(error);
            buffer.write(action);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            action_counter = buffer.read<uint32_t>();
            timer = buffer.read<float>();
            error = buffer.read<uint32_t>();
            action = buffer.read<uint32_t>();
        }
    };

}}}  // namespace swganh::messages::controllers


#endif  // SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_REMOVE_H_
