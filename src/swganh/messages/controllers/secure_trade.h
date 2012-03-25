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

#ifndef SWGANH_MESSAGES_CONTROLLERS_SECURE_TRADE_H_
#define SWGANH_MESSAGES_CONTROLLERS_SECURE_TRADE_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace controllers {
	
	class SecureTrade
	{
	public:
		static uint32_t header() { return 0x00000115; }
		
		uint32_t error;
		uint64_t trader_id; // sender
		uint64_t target_id; // recipient
		
		SecureTrade()
			: error(0)
			, trader_id(0)
			, target_id(0)
		{}
		
		void Serialize(anh::ByteBuffer& buffer) const
		{
			buffer.write(error);
			buffer.write(trader_id);
			buffer.write(target_id);
		}
		
		void Deserialize(anh::ByteBuffer buffer)
		{
			error = buffer.read<uint32_t>();
			trader_id = buffer.read<uint64_t>();
			target_id = buffer.read<uint64_t>();
		}
	};

}}} // namespace swganh::messages::controllers

#endif // SWGANH_MESSAGES_CONTROLLERS_SECURE_TRADE_H_