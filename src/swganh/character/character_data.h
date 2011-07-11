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
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace swganh {
namespace character {

struct CharacterData {
    std::wstring name;
    uint64_t character_id;
    uint32_t race_crc;
    uint32_t galaxy_id;
    uint32_t status;
};
struct CharacterLoginData {
    uint64_t character_id;
    uint32_t race_crc;
    glm::vec3 position;
    glm::quat orientation;
    std::string terrain_map;
    std::string race_template;
};
#define appearance_size 0x255
#define hair_customization_size 0x71
struct CharacterCreateInfo {
    uint32_t           account_id;
    uint64_t           character_id;
    std::string        first_name;
    std::string        last_name;
    std::string        base_model;
    std::string        profession;
    std::string        start_city;
    float              scale;
    std::string        biography;
    uint16_t           appearance[appearance_size];
    uint16_t           hair_customization[hair_customization_size];
    std::string        hair_model;
    std::string        hair_color;
};
//#undef appearance_size
//#undef hair_customization_size

}}  // namespace swganh::character

#endif  // SWGANH_CHARACTER_CHARACTER_DATA_H_
