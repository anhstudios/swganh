// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_REMOVE_BUFF_H_
#define SWGANH_MESSAGES_CONTROLLERS_REMOVE_BUFF_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "anh/hash_string.h"

#include "pub14_core/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class RemoveBuff : public ObjControllerMessage
    {
    public:
        explicit RemoveBuff(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
            , buff("")
        {}

        explicit RemoveBuff(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x0000022A; }
        
        anh::HashString buff;        
        
        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(buff);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            buff = buffer.read<uint32_t>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_REMOVE_BUFF_H_
