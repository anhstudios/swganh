// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_ADD_BUFF_H_
#define SWGANH_MESSAGES_CONTROLLERS_ADD_BUFF_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "anh/hash_string.h"

#include "pub14_core/messages/obj_controller_message.h"

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

        explicit AddBuff(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x00000229; }
        
        anh::HashString buff;        
        float duration;
        

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(buff);
            buffer.write(duration);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            buff = buffer.read<uint32_t>();
            duration = buffer.read<float>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_ADD_BUFF_H_
