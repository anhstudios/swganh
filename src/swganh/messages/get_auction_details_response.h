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

#ifndef SWGANH_MESSAGES_GET_AUCTION_DETAILS_RESPONSE_H_
#define SWGANH_MESSAGES_GET_AUCTION_DETAILS_RESPONSE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ItemDetail {
	std::string descriptor;
	std::wstring description;
}

struct GetAuctionDetailsResponse : public swganh::messages::BaseSwgMessage<GetAuctionDetailsResponse> {
	static uint16_t opcount() { return 9; }
	static uint32_t opcode() { return 0xFE0E644B; }
	
	uint64_t item_id;
	std::list<ItemDetail> auction_details;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(item_id);
		buffer.write(auction_details.size());
		std::for_each(auction_details.begin(), auction_details.end(), [&buffer] (ItemDetail detail) {
			buffer.write(detail.descriptor);
			buffer.write(detail.description);
		});
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		item_id = buffer.read<uint64_t>();
		uint32_t auction_details_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < auction_details_count; i++) {
			ItemDetail detail;
			detail.descriptor = buffer.read<std::string>();
			detail.description = buffer.read<std::wstring>();
			auction_details.push_back(detail);
		}
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_GET_AUCTION_DETAILS_RESPONSE_H_