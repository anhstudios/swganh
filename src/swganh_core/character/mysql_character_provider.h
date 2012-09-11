// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/character/character_provider_interface.h"

namespace swganh {
namespace app {
class KernelInterface;
}}  // namespace swganh::app

namespace swganh {
namespace character {

class MysqlCharacterProvider : public swganh::character::CharacterProviderInterface{
public:
    explicit MysqlCharacterProvider(swganh::app::KernelInterface* kernel);
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
	
    swganh::app::KernelInterface* kernel_;
	std::vector<std::string> reserved_names_;
	std::vector<std::string> developer_names_;
	std::vector<std::string> profane_names_;
	std::vector<std::string> fictionally_reserved_names_;
	std::vector<std::string> racially_inappropriate_;
};

}}  // namespace swganh::character
