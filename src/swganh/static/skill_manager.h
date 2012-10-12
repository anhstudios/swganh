// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <swganh_core/object/creature/creature.h>

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
	struct Skill
	{
		bool is_title;
		bool is_profession;
		uint32_t money_required;
		uint32_t skill_points_consumed;

		std::string xp_type;
		uint32_t xp_cost;
		uint32_t xp_cap;

		std::set<std::shared_ptr<Skill>> required_skills_;
		std::set<std::string> commands_;
		
		std::map<std::string, uint32_t> skill_mods_;
	};

	class SkillManager
	{
	public:
		SkillManager() {}

		void Start(std::shared_ptr<sql::Statement> statement);

		/*
		 * @brief Checks to see if the creature has access to the skill mod based on the given creature and it's skill levels
		 */
		virtual bool HasSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name);
		/*
		 * @brief Gets a given skill mod value if exists
		 */
		virtual uint32_t GetSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name);
		/*
		 * @brief Gets a given skill mod and any affected Attributes if exist
		 */
		virtual uint32_t GetTotalSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name);
		/*
		 * @brief Gets All SkillMods that are applicable for this creature.
		 */
		virtual std::map<std::string, uint32_t> GetSkillMods(const std::shared_ptr<swganh::object::Creature>& creature);

		virtual std::map<std::string, uint32_t> GetSkillModTotals(const std::shared_ptr<swganh::object::Creature>& creature);

	private:
		void _loadSkills(std::unique_ptr<sql::ResultSet> results);
		void _loadSkillPrereqs(std::unique_ptr<sql::ResultSet> results);
		void _loadSkillsRequiredSpecies(std::unique_ptr<sql::ResultSet> results);
		void _loadSkillMods(std::unique_ptr<sql::ResultSet> results);

		std::map<std::string, std::shared_ptr<Skill>> skills_;
	};
}
}