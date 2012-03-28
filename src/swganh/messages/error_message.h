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

#ifndef SWGANH_MESSAGES_ERROR_MESSAGE_H_
#define SWGANH_MESSAGES_ERROR_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ErrorMessage : public swganh::messages::BaseSwgMessage<ErrorMessage> {
	static uint16_t opcount() { return 3; }
	static uint32_t opcode() { return 0xB5ABF91A; }

	std::string type;
	std::string message;
	bool force_fatal;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(type);
		buffer.write(message);
		buffer.write(force_fatal);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		type = buffer.read<std::string>();
		message = buffer.read<std::string>();
		force_fatal = buffer.read<bool>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_ERROR_MESSAGE_H_