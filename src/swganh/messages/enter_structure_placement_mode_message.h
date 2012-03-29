// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_ENTER_STRUCTURE_PLACEMENT_MODE_MESSAGE_H_
#define SWGANH_MESSAGES_ENTER_STRUCTURE_PLACEMENT_MODE_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct EnterStructurePlacementModeMessage : public BaseSwgMessage<EnterStructurePlacementModeMessage>
    {
    	static uint16_t Opcount() { return 3; }
    	static uint32_t Opcode() { return 0xE8A54DC1; }

    	uint64_t deed_id;
    	std::string object_file_path; // e.g. "object/building/player/shared_player_garage_corellia_style_01.iff"

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(deed_id);
    		buffer.write(object_file_path);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		deed_id = buffer.read<uint64_t>();
    		object_file_path = buffer.read<std::string>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_ENTER_STRUCTURE_PLACEMENT_MODE_MESSAGE_H_
