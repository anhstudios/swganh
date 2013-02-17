// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

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
    std::string gender;
    std::string race;
    glm::vec3 position;
    glm::quat orientation;
    std::string terrain_map;
    std::string race_template;
    std::string shared_race_template;
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
