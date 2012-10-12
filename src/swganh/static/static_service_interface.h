// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "swganh/service/service_interface.h"

namespace swganh
{
namespace object
{
	class Creature;
}

namespace statics
{
	struct ElevatorData
	{
		uint64_t dst_cell;
		glm::quat dst_orientation;
		glm::vec3 dst_position;
		uint32_t effect_id;
		bool going_down;
	};

	class StaticServiceInterface : public swganh::service::ServiceInterface
	{
	public:

		//Returns the elevator data for a particular terminal.
		virtual std::vector<std::shared_ptr<ElevatorData>> GetElevatorDataForObject(uint64_t terminal_id) = 0;

		/*
		 * @brief Checks to see if the creature has access to the skill mod based on the given creature and it's skill levels
		 */
		virtual bool HasSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name) = 0;
		/*
		 * @brief Gets a given skill mod value if exists
		 */
		virtual uint32_t GetSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name) = 0;
		/*
		 * @brief Gets a given skill mod and any affected Attributes if exist
		 */
		virtual uint32_t GetTotalSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name) = 0;
		/*
		 * @brief Gets All SkillMods that are applicable for this creature.
		 */
		virtual std::map<std::string, uint32_t> GetSkillMods(const std::shared_ptr<swganh::object::Creature>& creature) = 0;

		virtual std::map<std::string, uint32_t> GetSkillModTotals(const std::shared_ptr<swganh::object::Creature>& creature) = 0;
	};
}
}