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

#ifndef CHARACTER_CHARACTER_SERVICE_H_
#define CHARACTER_CHARACTER_SERVICE_H_

#include "swganh/character/base_character_service.h"

#include "swganh/base/base_service.h"

namespace anh { namespace database { class DatabaseManagerInterface; } }

namespace character {
    
class CharacterService : public swganh::character::BaseCharacterService {
public:
    CharacterService(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher,
                     std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager);
    ~CharacterService();

    void onStart();
    void onStop();
    void Update();

    void subscribe();

    // CharacterService API Methods

    std::vector<swganh::character::CharacterData> GetCharactersForAccount(uint64_t account_id);
    swganh::character::CharacterLoginData GetLoginCharacter(uint64_t character_id);
    bool DeleteCharacter(uint64_t character_id);
    std::wstring GetRandomNameRequest(const std::string& base_model);
    bool UpdateCharacterStatus(uint64_t character_id, uint32_t status);
    std::tuple<uint64_t, std::string> CreateCharacter(const connection::messages::ClientCreateCharacter& character_info);
private:
    // helpers
    std::string parseAppearance_(std::string appearance_data);
    std::string parseBio_(const std::string& bio);
    std::string parseHair_(const std::string& hair_model, const std::string& hair_customization);
    std::string setCharacterCreateErrorCode_(uint32_t error_id);
    std::shared_ptr<anh::database::DatabaseManagerInterface> db_manager_;
};

}  // namespace character

#endif  // CHARACTER_CHARACTER_SERVICE_INTERFACE_H_
