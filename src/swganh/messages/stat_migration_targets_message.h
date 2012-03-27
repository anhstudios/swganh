/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_MESSAGES_STAT_MIGRATION_TARGETS_MESSAGE_H_
#define SWGANH_MESSAGES_STAT_MIGRATION_TARGETS_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct StatMigrationTargetsMessage : public swganh::messages::BaseSwgMessage<StatMigrationTargetsMessage> {
	static uint16_t opcount() { return 9; }
	static uint32_t opcode() { return 0xEFAC38C4; }

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

	void onSerialize(anh::ByteBuffer& buffer) const {
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

	void onDeserialize(anh::ByteBuffer buffer) {
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