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
 
#ifndef SWGANH_MESSAGES_CONTROLLERS_COMBAT_ACTION_MESSAGE_H_
#define SWGANH_MESSAGES_CONTROLLERS_COMBAT_ACTION_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"

#include "swganh/object/tangible/tangible.h"

namespace swganh {
namespace messages {
namespace controllers {

class CombatActionMessage
{
public:
	static uint32_t header() { return 0xCC; }

	CombatActionMessage()
		:defender_list(swganh::messages::containers::NetworkSortedVector<swganh::object::tangible::Defender>(25))
	{}
    
	uint32_t action_crc;
	uint64_t attacker_id;
	uint64_t weapon_id;
	uint8_t attacker_end_posture;
	uint8_t trails_bit_flag;
	uint8_t combat_special_move_effect;

	swganh::messages::containers::NetworkSortedVector<swganh::object::tangible::Defender>& defender_list;                         

	void Serialize(anh::ByteBuffer& buffer) const {
		buffer.write(action_crc);
		buffer.write(attacker_id);
		buffer.write(weapon_id);
		buffer.write(attacker_end_posture);
		buffer.write(trails_bit_flag);
		buffer.write(combat_special_move_effect);

		buffer.write(defender_list.Size());
		buffer.write<uint32_t>(0);
		std::for_each(defender_list.Begin(), defender_list.End(), [=, &buffer](swganh::object::tangible::Defender& defender){
			buffer.write(defender.object_id);
		});
	}

	void Deserialize(anh::ByteBuffer buffer) {
		action_crc = buffer.read<uint32_t>();
		attacker_id = buffer.read<uint64_t>();
		weapon_id = buffer.read<uint64_t>();
		attacker_end_posture = buffer.read<uint8_t>();
		trails_bit_flag = buffer.read<uint8_t>();
		combat_special_move_effect = buffer.read<uint8_t>();
		int defender_size = buffer.read<uint16_t>();
		for(int i = 0; i< defender_size; i++)
		{
			defender_list.Add(buffer.read<uint64_t>());
		}
	}
};

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_COMBAT_ACTION_MESSAGE_H_
