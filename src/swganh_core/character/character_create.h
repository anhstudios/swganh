// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <string>

namespace swganh {

namespace app { class SwganhKernel; }
namespace simulation { class SimulationServiceInterface; }

namespace character {

	class PyCharacterCreate
	{
	public:
		PyCharacterCreate(swganh::app::SwganhKernel* kernel);
			
		// Returns created character_id
		uint64_t CreateCharacter(const std::wstring& full_name, const std::string& profession, 
			const std::string& location, float height, const std::string& bio, const std::string& customization, 
			const std::string& hair_iff, const std::string& hair_customization, const std::string& iff_template);
	private:
		swganh::app::SwganhKernel* kernel_;
        swganh::simulation::SimulationServiceInterface* simulation_;
	};
	
}}