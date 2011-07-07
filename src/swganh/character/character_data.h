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

#ifndef SWGANH_CHARACTER_CHARACTER_DATA_H_
#define SWGANH_CHARACTER_CHARACTER_DATA_H_

#include <string>

namespace swganh {
namespace character {

struct CharacterData {
    std::wstring name;
    uint64_t character_id;
    uint32_t race_crc;
    uint32_t galaxy_id;
    uint32_t status;
};

}}  // namespace swganh::character

#endif  // SWGANH_CHARACTER_CHARACTER_DATA_H_
