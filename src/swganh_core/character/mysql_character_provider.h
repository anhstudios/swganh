// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/character/character_provider_interface.h"
#include "character_create.h"

namespace swganh {
namespace app {
class KernelInterface;
}}  // namespace swganh::app

namespace swganh {
namespace character {

/**
* A character provider that uses mysql to fetch characters
*/
class MysqlCharacterProvider : public swganh::character::CharacterProviderInterface{
public:
    explicit MysqlCharacterProvider(swganh::app::KernelInterface* kernel);
    ~MysqlCharacterProvider(){};

	/**
	* @param account_id the account to return the data for
	* @return all character data associated with this account.
	*/
    virtual std::vector<swganh::character::CharacterData> GetCharactersForAccount(uint64_t account_id);

	/**
	* @param character_id the character to delete
	* @param account_id the account to delete the character for
	*/
    virtual bool DeleteCharacter(uint64_t character_id, uint64_t account_id);
    
	/**
	* Creates a name based on the model given
	* @param base_model the model of the species to create the name for
	* @return a random name
	*/
	virtual std::wstring GetRandomNameRequest(const std::string& base_model);
    
	/**
	* @param player_id the account_id to get the max characters for
	* @return the max number of characters
	*/
	virtual uint16_t GetMaxCharacters(uint64_t player_id);

	/**
	* Creates a character based on the info provided
	* @param character_info the info of the character to create
	* @param account_id the id of the account to use
	* @return a tuple that indicates success or failure. 
	*   On success, it returns the object_id of the created character
	*   On failure, it returns the 0 and the string of the error that occurred
	*/
    virtual std::tuple<uint64_t, std::string> CreateCharacter(const swganh::messages::ClientCreateCharacter& character_info, uint32_t account_id);

	/**
	 * 
	 *
	 */
	
	/**
	* Checks if a particular name is allowed to be used
	* @param name the name to check
	* @return a tuple that indicates success or failure
	*   On success, it returns true
	*   On failure, it returns false and the reason why
	*/
	virtual std::tuple<bool, std::string> IsNameAllowed(std::string name);

	/**
	* Returns the object_id of the character with the given name
	* @param name the name to search for
	* @return the id of the character or 0 of not found
	*/
    virtual uint64_t GetCharacterIdByName(const std::string& name);

private:

	/**
	* @returns the string for the given mysql implementation-specific bad name error code
	*/
    std::string getCharacterCreateErrorCode_(uint32_t error_code);

	PyCharacterCreate py_character_create_;
	
    swganh::app::KernelInterface* kernel_;
	std::vector<std::string> reserved_names_;
	std::vector<std::string> developer_names_;
	std::vector<std::string> profane_names_;
	std::vector<std::string> fictionally_reserved_names_;
	std::vector<std::string> racially_inappropriate_;
};

}}  // namespace swganh::character
