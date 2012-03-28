
#ifndef SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_REMOVE_H_
#define SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_REMOVE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {

    class CommandQueueRemove
    {
    public:
        static uint32_t message_type() { return 0x00000117; }
        
        uint32_t action_counter;
        float timer;
        uint32_t error;
        uint32_t action;

        void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(action_counter);
            buffer.write(timer);
            buffer.write(error);
            buffer.write(action);
        }

        void Deserialize(anh::ByteBuffer buffer)
        {
            action_counter = buffer.read<uint32_t>();
            timer = buffer.read<float>();
            error = buffer.read<uint32_t>();
            action = buffer.read<uint32_t>();
        }
    };

}}}  // namespace swganh::messages::controllers


#endif  // SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_REMOVE_H_
