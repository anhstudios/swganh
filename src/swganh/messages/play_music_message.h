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

#ifndef SWGANH_MESSAGES_PLAY_MUSIC_MESSAGE_H_
#define SWGANH_MESSAGES_PLAY_MUSIC_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct PlayMusicMessage : public swganh::messages::BaseSwgMessage<PlayMusicMessage> {
	static uint16_t opcount() { return 5; }
	static uint32_t opcode() { return 0x04270D8A; }
	
	std::string music_file; // e.g. "sound/music_id_tent_naboo_loop.snd"
	uint64_t unknown1;
	uint32_t unknown2;
	uint8_t unknown3;
	
	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(music_file);
		buffer.write(unknown1);
		buffer.write(unknown2);
		buffer.write(unknown3);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		music_file = buffer.read<std::string>();
		unknown1 = buffer.read<uint64_t>();
		unknown2 = buffer.read<uint32_t>();
		unknown3 = buffer.read<uint8_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PLAY_MUSIC_MESSAGE_H_