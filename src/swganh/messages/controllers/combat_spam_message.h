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
 
#ifndef SWGANH_MESSAGES_CONTROLLERS_COMBAT_SPAM_MESSAGE_H_
#define SWGANH_MESSAGES_CONTROLLERS_COMBAT_SPAM_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {

class CombatSpamMessage
{
public:
	static uint32_t message_type() { return 0x134; }

	CombatSpamMessage()
        : attacker_id(0)
        , defender_id(0)
        , weapon_id(0)
        , damage(0)
        , file("")
        , text("cbt_spam")
        , color_flag(1)
        , unicode_string(L"")
	{}
    
	uint64_t attacker_id;
    uint64_t defender_id;
	uint64_t weapon_id;
    int32_t damage;
	std::string file;
	std::string text;
	uint8_t color_flag;
    std::wstring unicode_string;

	void Serialize(anh::ByteBuffer& buffer) const {
		buffer.write(attacker_id);
		buffer.write(defender_id);
		buffer.write(weapon_id);
		buffer.write(damage);
		buffer.write(file);
		buffer.write(0);
        buffer.write(text);
        buffer.write(color_flag);
        buffer.write(unicode_string);

	}

	void Deserialize(anh::ByteBuffer buffer) {
		attacker_id = buffer.read<uint64_t>();
		defender_id = buffer.read<uint64_t>();
		weapon_id = buffer.read<uint64_t>();
		damage = buffer.read<int>();
		file = buffer.read<std::string>();
        buffer.read<uint32_t>();
		text = buffer.read<std::string>();
        color_flag = buffer.read<uint8_t>();
        unicode_string = buffer.read<std::wstring>();
	}
};

}}}  // namespace swganh::messages::controllers

#endif  // SWGANH_MESSAGES_CONTROLLERS_COMBAT_SPAM_MESSAGE_H_
