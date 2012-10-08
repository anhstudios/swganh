// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include "swganh/character/character_data.h"
#include "swganh/messages/client_create_character.h"

namespace swganh {
namespace character {
    
class CharacterProviderInterface {
public:
    virtual ~CharacterProviderInterface() {}
    
};

}}  // namespace swganh::character
