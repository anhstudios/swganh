// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

#include "anh/byte_buffer.h"

#include "base_swg_message.h"

namespace swganh {
namespace messages {

    class ObjControllerMessage : public BaseSwgMessage<ObjControllerMessage>
    {
    public:
        static uint16_t Opcount() { return 5; }
        static uint32_t Opcode() { return 0x80CE5E46; }

        ObjControllerMessage()
        {}

        ObjControllerMessage(uint32_t controller_type_, uint32_t message_type_)
        {
            controller_type = controller_type_;
            message_type = message_type_;
        }

        uint32_t controller_type;
        uint32_t message_type;
        uint64_t observable_id;
        uint32_t tick_count;
        anh::ByteBuffer data;
        
        virtual void OnControllerSerialize(anh::ByteBuffer& buffer) const 
        {
            buffer.write(data.data(), data.size());
        }

        virtual void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            data = std::move(buffer);
        }

        virtual void OnSerialize(anh::ByteBuffer& buffer) const 
        {
            buffer.write(controller_type);
            buffer.write(message_type);
            buffer.write(observable_id);
            buffer.write(tick_count);

            OnControllerSerialize(buffer);
        }

        virtual void OnDeserialize(anh::ByteBuffer buffer) 
        {
            controller_type = buffer.read<uint32_t>();
            message_type = buffer.read<uint32_t>();  
            observable_id = buffer.read<uint64_t>();
            tick_count = buffer.read<uint32_t>();
            OnControllerDeserialize(std::move(buffer));
        }
    };

}}  // namespace swganh::messages
