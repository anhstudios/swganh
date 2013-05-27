// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <string>
#include <cstdint>

namespace swganh
{
namespace chat
{

class ChatUserProviderInterface
{
public:

	/*!
	* @param name the full name of the user
	* @return the user's creature's id
	*/
	virtual uint64_t GetIdFromFullName(const std::string& name) const = 0;
	
	/*!
	* @param name the first name of the user
	* @param the user's creature's id
	*/
	virtual uint64_t GetIdFromFirstName(const std::string& name) const = 0;

	/*!
	* @param creature_id the id of the user's creature
	* @return the full name of the user
	*/
	virtual const std::string& GetFullNameFromId(uint64_t creature_id) const = 0;
	
	/*!
	* @param creature_id the id of the user's creature
	* @return the first name of the user
	*/
	virtual std::string GetFirstNameFromId(uint64_t creature_id) const = 0;
	
	/*!
	* @param name the first name of the player requested
	* @return the full name of the player.
	*/
	const std::string& GetFullNameFromFirstName(const std::string& name) const;

	/*!
	* @param creature_id the id of the user's creature
	* @return the full path of the user
	*/
	virtual std::string GetUsernamePathFromId(uint64_t creature_id) const = 0;
	
	/*!
	* @param name the full name of the user
	* @return the first name of the user
	*/
	virtual std::string GetFirstNameFromFullName(const std::string& name) const = 0;
	
};

}
}