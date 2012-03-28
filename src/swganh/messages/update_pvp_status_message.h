// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_UPDATE_PVP_STATUS_MESSAGE_H_
#define SWGANH_MESSAGES_UPDATE_PVP_STATUS_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
    struct UpdatePvpStatusMessage : public BaseSwgMessage<UpdatePvpStatusMessage>
    {
        static uint16_t Opcount() { return 4; }
        static uint32_t Opcode() { return 0x08A1C126; }
        
        uint32_t pvp_status;
        uint32_t faction;
        uint64_t object_id;
        
        void OnSerialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(pvp_status);
            buffer.write(faction);
            buffer.write(object_id);
        }

        void OnDeserialize(anh::ByteBuffer buffer)
        {
            pvp_status = buffer.read<uint32_t>();
            faction = buffer.read<uint32_t>();
            object_id = buffer.read<uint64_t>();
        }
    };

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_UPDATE_PVP_STATUS_MESSAGE_H_
