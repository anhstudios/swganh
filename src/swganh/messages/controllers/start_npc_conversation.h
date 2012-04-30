// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_CONTROLLERS_START_NPC_CONVERSATION_H_
#define SWGANH_MESSAGES_CONTROLLERS_START_NPC_CONVERSATION_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

#include "swganh/messages/obj_controller_message.h"

namespace swganh {
namespace messages {
namespace controllers {

    class StartNPCConversation : public ObjControllerMessage
    {
    public:
        explicit StartNPCConversation(uint32_t controller_type = 0x0000000B)
            : ObjControllerMessage(controller_type, message_type())
            , npc_id(0)
        {}

        explicit StartNPCConversation(ObjControllerMessage controller_message)
            : ObjControllerMessage(std::move(controller_message))
        {
            OnControllerDeserialize(std::move(data));
        }

        static uint32_t message_type() { return 0x000000DD; }
        
        uint64_t npc_id;        
        

        void OnControllerSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(npc_id);
            // @TODO Unknown
            buffer.write<uint32_t>(0);
            buffer.write<uint16_t>(0);
            buffer.write<uint8_t>(0);
        }

        void OnControllerDeserialize(anh::ByteBuffer buffer)
        {
            npc_id = buffer.read<uint64_t>();
        }
    };

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_START_NPC_CONVERSATION_H_
