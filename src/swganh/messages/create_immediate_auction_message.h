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

#ifndef SWGANH_MESSAGES_CREATE_IMMEDIATE_AUCTION_MESSAGE_H_
#define SWGANH_MESSAGES_CREATE_IMMEDIATE_AUCTION_MESSAGE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct CreateImmediateAuctionMessage : public swganh::messages::BaseSwgMessage<CreateImmediateAuctionMessage> {
	static uint16_t opcount() { return 8; }
	static uint32_t opcode() { return 0x1E9CE308; }

	uint64_t item_id;
	uint64_t bazaar_terminal_id;
	uint32_t price;
	uint32_t auction_duration_seconds;
	std::wstring item_description;
	uint8_t premium_auction_flag; // 0 = non-premium, 1 = premium
	
	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(item_id);
		buffer.write(bazaar_terminal_id);
		buffer.write(price);
		buffer.write(auction_duration_seconds);
		buffer.write(item_description);
		buffer.write(premium_auction_flag);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		item_id = buffer.read<uint64_t>();
		bazaar_terminal_id = buffer.read<uint64_t>();
		price = buffer.read<uint32_t>();
		auction_duration_seconds = buffer.read<uint32_t>();
		item_description = buffer.read<std::wstring>();
		premium_auction_flag = buffer.read<uint8_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CREATE_IMMEDIATE_AUCTION_MESSAGE_H_