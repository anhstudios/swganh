// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

namespace swganh {
namespace app {
	class SwganhKernel;
}
namespace character {

	class PyCharacterCreate
	{
		PyCharacterCreate(swganh::app::SwganhKernel* kernel);

		bool CreateCharacter(uint64_t character_id, const std::string& full_name
	};
	
}}