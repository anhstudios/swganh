// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_STAT_MIGRATION_TARGETS_MESSAGE_H_
#define SWGANH_MESSAGES_STAT_MIGRATION_TARGETS_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct StatMigrationTargetsMessage : public BaseSwgMessage<StatMigrationTargetsMessage>
    {
    	static uint16_t Opcount() { return 9; }
    	static uint32_t Opcode() { return 0xEFAC38C4; }

    	uint32_t health_points;
    	uint32_t strength_points;
    	uint32_t constitution_points;
    	uint32_t action_points;
    	uint32_t quickness_points;
    	uint32_t stamina_points;
    	uint32_t mind_points;
    	uint32_t focus_points;
    	uint32_t willpower_points;
    	uint32_t points_remaining;

    	void OnSerialize(anh::ByteBuffer& buffer) const
    	{
    		buffer.write(health_points);
    		buffer.write(strength_points);
    		buffer.write(constitution_points);
    		buffer.write(action_points);
    		buffer.write(quickness_points);
    		buffer.write(stamina_points);
    		buffer.write(mind_points);
    		buffer.write(focus_points);
    		buffer.write(willpower_points);
    		buffer.write(points_remaining);
    	}

    	void OnDeserialize(anh::ByteBuffer buffer)
    	{
    		health_points = buffer.read<uint32_t>();
    		strength_points = buffer.read<uint32_t>();
    		constitution_points = buffer.read<uint32_t>();
    		action_points = buffer.read<uint32_t>();
    		quickness_points = buffer.read<uint32_t>();
    		stamina_points = buffer.read<uint32_t>();
    		mind_points = buffer.read<uint32_t>();
    		focus_points = buffer.read<uint32_t>();
    		willpower_points = buffer.read<uint32_t>();
    		points_remaining = buffer.read<uint32_t>();
    	}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_STAT_MIGRATION_TARGETS_MESSAGE_H_