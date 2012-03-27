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

#ifndef SWGANH_MESSAGES_GET_MAP_LOCATIONS_RESPONSE_MESSAGE_H_
#define SWGANH_MESSAGES_GET_MAP_LOCATIONS_RESPONSE_MESSAGE_H_

#include <cstdint>
#include <algorithm>
#include <list>
#include <string>
#include "anh/byte_buffer.h"
#include "swganh/messages/base_swg_message.h"

namespace swganh {
namespace messages {

struct MapLocation {
	uint64_t id;
	std::wstring name;
	float x;
	float y;
	// use only one of the following; choose the type to use and assign it to the appropriate display mode
	uint8_t type_displayAsCategory;
	uint8_t type_displayAsSubcategory;
	uint8_t type_displayAsActive;
}

struct GetMapLocationsResponseMessage : public swganh::messages::BaseSwgMessage<GetMapLocationsResponseMessage> {
	static uint16_t opcount() { return 28; }
	static uint32_t opcode() { return 0x9F80464C; }
	
	std::string planet_name;
	std::list<MapLocation> locations;
	std::list<MapLocation> blank_list1;
	std::list<MapLocation> blank_list2;

	void onSerialize(anh::ByteBuffer& buffer) const {
		buffer.write(planet_name);
		buffer.write(locations.size());
		std::for_each(locations.begin(), locations.end(), [&buffer] (MapLocation location) {
			buffer.write(location.id);
			buffer.write(location.name);
			buffer.write(location.x);
			buffer.write(location.y);
			buffer.write(location.type_displayAsCategory);
			buffer.write(location.type_displayAsSubcategory);
			buffer.write(location.type_displayAsActive);
		});
		// unclear why the following lists are needed; locations will be displayed no matter the list
		buffer.write(blank_list1.size());
		std::for_each(blank_list1.begin(), blank_list1.end(), [&buffer] (MapLocation location) {
			buffer.write(location.id);
			buffer.write(location.name);
			buffer.write(location.x);
			buffer.write(location.y);
			buffer.write(location.type_displayAsCategory);
			buffer.write(location.type_displayAsSubcategory);
			buffer.write(location.type_displayAsActive);
		});
		buffer.write(blank_list2.size());
		std::for_each(blank_list2.begin(), blank_list2.end(), [&buffer] (MapLocation location) {
			buffer.write(location.id);
			buffer.write(location.name);
			buffer.write(location.x);
			buffer.write(location.y);
			buffer.write(location.type_displayAsCategory);
			buffer.write(location.type_displayAsSubcategory);
			buffer.write(location.type_displayAsActive);
		});
	}

	void onDeserialize(anh::ByteBuffer buffer) {
		planet_name = buffer.read<std::string>();
		uint32_t locations_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < locations_count; i++) {
			MapLocation location;
			location.id = buffer.read<uint64_t>();
			location.name = buffer.read<std::wstring>();
			location.x = buffer.read<float>();
			location.y = buffer.read<float>();
			location.type_displayAsCategory = buffer.read<uint8_t>();
			location.type_displayAsSubcategory = buffer.read<uint8_t>();
			location.type_displayAsActive = buffer.read<uint8_t>();
			locations.push_back(location);
		}
		// unclear why the following lists are needed; locations will be displayed no matter the list
		uint32_t blank_list1_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < blank_list1_count; i++) {
			MapLocation location;
			location.id = buffer.read<uint64_t>();
			location.name = buffer.read<std::wstring>();
			location.x = buffer.read<float>();
			location.y = buffer.read<float>();
			location.type_displayAsCategory = buffer.read<uint8_t>();
			location.type_displayAsSubcategory = buffer.read<uint8_t>();
			location.type_displayAsActive = buffer.read<uint8_t>();
			blank_list1.push_back(location);
		}
		uint32_t blank_list2_count = buffer.read<uint32_t>();
		for (uint32_t i = 0; i < blank_list2_count; i++) {
			MapLocation location;
			location.id = buffer.read<uint64_t>();
			location.name = buffer.read<std::wstring>();
			location.x = buffer.read<float>();
			location.y = buffer.read<float>();
			location.type_displayAsCategory = buffer.read<uint8_t>();
			location.type_displayAsSubcategory = buffer.read<uint8_t>();
			location.type_displayAsActive = buffer.read<uint8_t>();
			blank_list2.push_back(location);
		}
	}
};

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_GET_MAP_LOCATIONS_RESPONSE_MESSAGE_H_