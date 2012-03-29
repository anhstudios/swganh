
#ifndef SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_ENQUEUE_H_
#define SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_ENQUEUE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {

    class CommandQueueEnqueue
    {
    public:
        static uint32_t message_type() { return 0x00000116; }
        
        uint32_t action_counter;
        uint32_t command_crc;
        uint64_t target_id;
        std::wstring command_options;

        void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(action_counter);
            buffer.write(command_crc);
            buffer.write(target_id);
            buffer.write(command_options);
        }

        void Deserialize(anh::ByteBuffer buffer)
        {
            action_counter = buffer.read<uint32_t>();
            command_crc = buffer.read<uint32_t>();
            target_id = buffer.read<uint64_t>();
            command_options = buffer.read<std::wstring>();
        }
    };

}}}  // namespace swganh::messages::controllers


#endif  // SWGANH_MESSAGES_CONTROLLERS_COMMAND_QUEUE_ENQUEUE_H_
