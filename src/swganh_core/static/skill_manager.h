// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <map>
#include <set>
#include <string>
#include <memory>

#include "skill.h"
#include "skill_mod.h"
#include "skill_info.h"

namespace sql
{
	class Statement;
	class ResultSet;
}

namespace swganh
{

namespace object
{
	class Creature;
}

namespace statics
{
	typedef std::map<std::string, swganh::object::SkillMod> SkillModsMap;
	class SkillManager
	{
	public:
		SkillManager() {}

		void Start(std::shared_ptr<sql::Statement> statement);

		/**
		* @return all information regarding a particular skill.
		*/
		std::shared_ptr<SkillInfo> GetSkillInfo(const std::string& name);

		/*
		 * @brief Gets a given skill mod and any affected Attributes if exist
		 * @return a pair of base, modifier
		 */
		swganh::object::SkillMod GetSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name);
		
		/*
		 * @brief Gets All SkillMods that are applicable for this creature.
		 * @return a map of pairs of base, modifier
		 */
		SkillModsMap GetSkillMods(const std::shared_ptr<swganh::object::Creature>& creature);

	private:
		void _loadSkills(std::unique_ptr<sql::ResultSet> results);
		void _loadSkillPrereqs(std::unique_ptr<sql::ResultSet> results);
		void _loadSkillsRequiredSpecies(std::unique_ptr<sql::ResultSet> results);
		void _loadSkillMods(std::unique_ptr<sql::ResultSet> results);
		void _loadSkillCommands(std::unique_ptr<sql::ResultSet> results);

		std::map<std::string, std::shared_ptr<SkillInfo>> skills_;
	};
}
}