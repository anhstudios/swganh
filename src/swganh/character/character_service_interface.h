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

#ifndef SWGANH_CHARACTER_CHARACTER_SERVICE_INTERFACE_H_
#define SWGANH_CHARACTER_CHARACTER_SERVICE_INTERFACE_H_

#include <cstdint>
#include <vector>

#include "swganh/base/base_service.h"

#include "swganh/character/character_data.h"

namespace swganh {
namespace character {
    
class CharacterServiceInterface : public swganh::base::BaseService {
public:
    virtual ~CharacterServiceInterface() {}
    
    virtual std::vector<character::CharacterData> GetCharactersForAccount(uint64_t account_id) = 0;
};

}}  // namespace swganh::character

#endif  // SWGANH_CHARACTER_CHARACTER_SERVICE_INTERFACE_H_

