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

#ifndef SWGANH_MESSAGES_CREATE_AUCTION_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_CREATE_AUCTION_RESPONSE_MESSAGE_H_

#include <cstdint>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct CreateAuctionResponseMessage : public swganh::messages::BaseSwgMessage<CreateAuctionResponseMessage> {
	static uint16_t opcount() { return 3; }
	static uint32_t opcode() { return 0xE61CC92; }
	
	uint64_t unknown; // default: 0
	uint32_t response_type; // setting this determines what message will display on the client
	
	/*
		response_type choices:

		0: The item has been put up for sale
		1: You cannot sell that item because the auctioner is invalid
		2: You cannot sell an invalid item
		3: Sale failed because the vendor is malfunctioning
		4: You must specify a valid sale price
		5: Invalid sale duration
		6: The item is already for sale
		7: An unknown error occured while creating the sale
		8: You cannot sell an item you do not own
		9: You don't have enough credits to post that Sale. The Commodities Market requires 5 credits.
		10: An unknown error occured while creating the sale
		11: An unknown error occured while creating the sale
		12: An unknown error occured while creating the sale
		13: You have too many auctions on the commodities market. Please withdraw some sales and pick up any items waiting in the Available Items tab.
		14: Items can not be sold on the bazaar for more than 6000 credits. 
		15: An unknown error occured while creating the sale
		16: This vendor is currently deactivated and is not accepting new items to be placed up for sale or offer. If you are the owner of this vendor, please make sure the maintenance fees for this vendor have been paid to reactivate the vendor.
		17: An unknown error occured while creating the sale
		18: An unknown error occured while creating the sale
		19: You cannot sell an item that you are trading.
		20: You are not allowed to sell an item in a crate.
		21: You are not allowed to sell that item.
		22: You can only sell empty containers.
		23: An unknown error occured while creating the sale
		24: You have more vendors than you can manage. You must either gain sufficient skill to manage the vendors you have, or remove one or more vendors before being allowed to place items for sale.
		25: This vendor is at maximum capacity and cannot accept any more items for sale.
		26: An unknown error occured while creating the sale
		27: An unknown error occured while creating the sale
		28: An unknown error occured while creating the sale
		29: An unknown error occured while creating the sale
	*/

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(unknown);
		buffer.write(response_type);
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		unknown = buffer.read<uint64_t>();
		response_type = buffer.read<uint32_t>();
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_CREATE_AUCTION_RESPONSE_MESSAGE_H_