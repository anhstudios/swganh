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

#ifndef MYSQL_CHARACTER_PROVIDER_H_
#define MYSQL_CHARACTER_PROVIDER_H_

#include "swganh/character/character_provider_interface.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

namespace plugins {
namespace mysql_character {

class MysqlCharacterProvider : public swganh::character::CharacterProviderInterface{
public:
    explicit MysqlCharacterProvider(anh::app::KernelInterface* kernel);
    ~MysqlCharacterProvider(){};

    virtual std::vector<swganh::character::CharacterData> GetCharactersForAccount(uint64_t account_id);
    virtual bool DeleteCharacter(uint64_t character_id, uint64_t account_id);
    virtual std::wstring GetRandomNameRequest(const std::string& base_model);
    virtual uint16_t GetMaxCharacters(uint64_t player_id);
    virtual std::tuple<uint64_t, std::string> CreateCharacter(const swganh::messages::ClientCreateCharacter& character_info, uint32_t account_id);
	virtual std::tuple<bool, std::string> IsNameAllowed(std::string name);
    virtual uint64_t GetCharacterIdByName(const std::string& name);

private:
    std::string setCharacterCreateErrorCode_(uint32_t error_code);
	
    anh::app::KernelInterface* kernel_;
	std::vector<std::string> reserved_names_;
	std::vector<std::string> developer_names_;
	std::vector<std::string> profane_names_;
	std::vector<std::string> fictionally_reserved_names_;
	std::vector<std::string> racially_inappropriate_;
};

}}  // namespace plugins::mysql_character

#endif  // MYSQL_CHARACTER_PROVIDER_H_