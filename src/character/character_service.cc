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

#include "character/character_service.h"

using namespace anh;
using namespace swganh::character;
using namespace event_dispatcher;
using namespace character;
using namespace std;

CharacterService::CharacterService(shared_ptr<EventDispatcherInterface> dispatcher) {
    event_dispatcher(dispatcher);
}

CharacterService::~CharacterService() {}

void CharacterService::subscribe() {}

void CharacterService::onStart() {}

void CharacterService::onStop() {}

void CharacterService::Update() {}

vector<CharacterData> CharacterService::GetCharactersForAccount(uint64_t account_id) {
    vector<CharacterData> characters;

    CharacterData character;
    character.character_id = 1;
    character.name = L"Apathy";
    character.race_crc = 0x060E51D5;
    character.galaxy_id = 2;
    character.status = 1;

    characters.push_back(character);

    return characters;
}
