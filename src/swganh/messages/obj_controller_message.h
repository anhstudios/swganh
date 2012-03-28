// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_OBJ_CONTROLLER_MESSAGE_H_
#define SWGANH_MESSAGES_OBJ_CONTROLLER_MESSAGE_H_

#include <cstdint>

#include "anh/byte_buffer.h"

#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct ObjControllerMessage : public BaseSwgMessage<ObjControllerMessage>
    {
        static uint16_t Opcount() { return 5; }
        static uint32_t Opcode() { return 0x80CE5E46; }

        ObjControllerMessage()
        {}

        template<typename T>
        ObjControllerMessage(uint32_t controller_type, const T& payload)
        {
            controller_type = controller_type;
            tick_count = 0;
            message_type = T::message_type();
            payload.Serialize(data);
        }

        uint32_t controller_type;
        uint32_t message_type;
        uint64_t observable_id;
        uint32_t tick_count;
        anh::ByteBuffer data;
        
        void OnSerialize(anh::ByteBuffer& buffer) const 
        {
            buffer.write(controller_type);
            buffer.write(message_type);
            buffer.write(observable_id);
            buffer.write(tick_count);
            buffer.write(data.data(), data.size());  
        }

        void OnDeserialize(anh::ByteBuffer buffer) 
        {
            controller_type = buffer.read<uint32_t>();
            message_type = buffer.read<uint32_t>();  
            observable_id = buffer.read<uint64_t>();
            tick_count = buffer.read<uint32_t>();
            data = anh::ByteBuffer(buffer.data() + buffer.read_position(), buffer.size() - buffer.read_position());
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_OBJ_CONTROLLER_MESSAGE_H_
