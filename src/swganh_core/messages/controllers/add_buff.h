// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>
#include "swganh/byte_buffer.h"
#include "swganh/hash_string.h"

#include "swganh_core/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class AddBuff : public ObjControllerMessage
    {
    public:
        explicit AddBuff(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
            , buff("")
            , duration(0.0f)
        {}

		AddBuff(const ObjControllerMessage& base)
			: ObjControllerMessage(base)
		{
		}

        static uint32_t message_type() { return 0x00000229; }
        
        swganh::HashString buff;        
        float duration;
        

        void OnControllerSerialize(swganh::ByteBuffer& buffer) const
        {
            buffer.write(buff);
            buffer.write(duration);
        }

        void OnControllerDeserialize(swganh::ByteBuffer& buffer)
        {
            buff = buffer.read<uint32_t>();
            duration = buffer.read<float>();
        }
    };

}}}  // namespace swganh::messages::controllers
