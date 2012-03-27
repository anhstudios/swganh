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

#ifndef SWGANH_MESSAGES_LOGIN_CLIENT_ID_H_
#define SWGANH_MESSAGES_LOGIN_CLIENT_ID_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct LoginClientId : public swganh::messages::BaseSwgMessage<LoginClientId> {
	static uint16_t opcount() { return 4; }
	static uint32_t opcode() { return 0x41131F96; }

	std::string username;
	std::string password;
	std::string client_version; // <year><month><day>-<hour>:<minute> for publish, e.g. Publish 13.0 = 20050125-12:19

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(username);
		buffer.write(password);
		buffer.write(client_version);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		username = buffer.read<std::string>();
		password = buffer.read<std::string>();
		client_version = buffer.read<std::string>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_LOGIN_CLIENT_ID_H_