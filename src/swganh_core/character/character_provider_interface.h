// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include "swganh_core/character/character_data.h"
#include "swganh_core/messages/client_create_character.h"

namespace swganh {
namespace character {
    
class CharacterProviderInterface {
public:
    virtual ~CharacterProviderInterface() {}
    
    virtual std::vector<CharacterData> GetCharactersForAccount(uint64_t account_id) = 0;
    virtual bool DeleteCharacter(uint64_t character_id, uint64_t account_id) = 0;
    virtual std::wstring GetRandomNameRequest(const std::string& base_model) = 0;
    virtual std::tuple<uint64_t, std::string> CreateCharacter(const swganh::messages::ClientCreateCharacter& character_info, uint32_t account_id) = 0;
    virtual uint16_t GetMaxCharacters(uint64_t player_id) = 0;
	virtual std::tuple<bool, std::string> IsNameAllowed(std::string name) = 0;
    
	virtual uint64_t GetCharacterIdByName(const std::string& name) = 0;
};

}}  // namespace swganh::character
