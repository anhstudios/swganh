// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_ENTERTAINER_FLOURISH_H_
#define SWGANH_MESSAGES_CONTROLLERS_ENTERTAINER_FLOURISH_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class EntertainerFlourish : public ObjControllerMessage
    {
    public:
        explicit EntertainerFlourish(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
            , flourish_id(0)
        {}

        explicit EntertainerFlourish(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x00000166; }
        
        uint32_t flourish_id;
        

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write<uint32_t>(0);
            buffer.write(flourish_id);
            buffer.write<uint32_t>(0);
            buffer.write<uint32_t>(0);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            flourish_id = buffer.read<uint32_t>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_ENTERTAINER_FLOURISH_H_
