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

#ifndef SWGANH_MESSAGES_AUCTION_QUERY_HEADERS_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_AUCTION_QUERY_HEADERS_RESPONSE_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct ItemInfo {
	uint64_t item_id;
	uint8_t item_string_number;
	uint32_t price1;
	uint32_t expiration_time;
	uint32_t auction_flag;
	uint16_t location_index;
	uint64_t seller_object_id;
	uint8_t seller_pointer; // unknown
	uint64_t buyer_object_id;
	uint8_t buyer_pointer; // uncertain
	uint32_t unknown1; // unknown
	uint32_t price2;
	uint32_t item_type;
	uint32_t unknown2; // unknown
	uint32_t vendor_entrance_fee;
};

struct AuctionQueryHeadersResponseMessage : public swganh::messages::BaseSwgMessage<AuctionQueryHeadersResponseMessage> {
	static uint16_t opcount() { return 8; }
	static uint32_t opcode() { return 0xFA500E52; }

	uint32_t counter;
	uint32_t vendor_screen_flag; // 2 = All items, 3 = My sales, 4 = My bids, 5 = Available items, 7 = For sale, 9 = Offers to vendor
	std::list<std::string> location_names;
	std::list<std::wstring> item_names;
	std::list<ItemInfo> item_info_list;
	uint16_t item_list_start_offset; // WHen the continuation flag is enabled, this offset could have a value other than 0
	uint8_t continuation_flag; // Determines whether or not more items are to be added to the list

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(counter);
		buffer.write(vendor_screen_flag);
		buffer.write<uint32_t>(location_names.size());
		std::for_each(location_names.begin(), location_names.end(), [&buffer] (std::string location_name) {
			buffer.write(location_name);
		});
		buffer.write<uint32_t>(item_names.size());
		std::for_each(item_names.begin(), item_names.end(), [&buffer] (std::wstring item_name) {
			buffer.write(item_name);
		});
		buffer.write<uint32_t>(item_info_list.size());
		std::for_each(item_info_list.begin(), item_info_list.end(), [&buffer] (ItemInfo item) {
			buffer.write(item.item_id);
			buffer.write(item.item_string_number);
			buffer.write(item.price1);
			buffer.write(item.expiration_time);
			buffer.write(item.auction_flag);
			buffer.write(item.location_index);
			buffer.write(item.seller_object_id);
			buffer.write(item.seller_pointer);
			buffer.write(item.buyer_object_id);
			buffer.write(item.buyer_pointer);
			buffer.write(item.unknown1);
			buffer.write(item.price2);
			buffer.write(item.item_type);
			buffer.write(item.unknown2);
			buffer.write(item.vendor_entrance_fee);
		});
		buffer.write(item_list_start_offset);
		buffer.write(continuation_flag);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
        counter = buffer.read<uint32_t>();
		vendor_screen_flag = buffer.read<uint32_t>();
		uint32_t location_names_count = buffer.read<uint32_t>();
		for(uint32_t i = 0; i < location_names_count; i++) {
			std::string location_name;
			location_name = buffer.read<std::string>();
			location_names.push_back(location_name);
		}
		uint32_t item_names_count = buffer.read<uint32_t>();
		for(uint32_t i = 0; i < item_names_count; i++) {
			std::wstring item_name;
			item_name = buffer.read<std::wstring>();
			item_names.push_back(item_name);
		}
		uint32_t item_info_count = buffer.read<uint32_t>();
		for(uint32_t i = 0; i < item_info_count; i++) {
			ItemInfo item;
			item.item_id = buffer.read<uint64_t>();
			item.item_string_number = buffer.read<uint8_t>();
			item.price1 = buffer.read<uint32_t>();
			item.expiration_time = buffer.read<uint32_t>();
			item.auction_flag = buffer.read<uint32_t>();
			item.location_index = buffer.read<uint16_t>();
			item.seller_object_id = buffer.read<uint64_t>();
			item.seller_pointer = buffer.read<uint8_t>();
			item.buyer_object_id = buffer.read<uint64_t>();
			item.buyer_pointer = buffer.read<uint8_t>();
			item.unknown1 = buffer.read<uint32_t>();
			item.price2 = buffer.read<uint32_t>();
			item.item_type = buffer.read<uint32_t>();
			item.unknown2 = buffer.read<uint32_t>();
			item.vendor_entrance_fee = buffer.read<uint32_t>();
			item_info_list.push_back(item);
		}
		item_list_start_offset = buffer.read<uint16_t>();
		continuation_flag = buffer.read<uint8_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_AUCTION_QUERY_HEADERS_RESPONSE_MESSAGE_H_