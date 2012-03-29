// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_UPDATE_CELL_PERMISSION_MESSAGE_H_
#define SWGANH_MESSAGES_UPDATE_CELL_PERMISSION_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {
    
struct UpdateCellPermissionMessage : public BaseSwgMessage<UpdateCellPermissionMessage>
{
    static uint16_t Opcount() { return 3; }
    static uint32_t Opcode() { return 0xF612499C; }
    
    uint8_t permission_flag;
    uint64_t cell_id;
    
    void OnSerialize(anh::ByteBuffer& buffer) const
    {
        buffer.write(permission_flag);
        buffer.write(cell_id);
    }

    void OnDeserialize(anh::ByteBuffer buffer)
    {
        permission_flag = buffer.read<uint8_t>();
        cell_id = buffer.read<uint64_t>();
    }
};

}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_UPDATE_CELL_PERMISSION_MESSAGE_H_
