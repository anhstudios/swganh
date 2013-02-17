// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "skill_manager.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

using namespace swganh::statics;
using namespace swganh::object;

void SkillManager::Start(std::shared_ptr<sql::Statement> statement)
{
	_loadSkills(std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())));

	statement->getMoreResults();
	_loadSkillPrereqs(std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())));

	//statement->getMoreResults();
	//_loadSkillsRequiredSpecies(std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())));

	statement->getMoreResults();
	_loadSkillMods(std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())));

}

void SkillManager::_loadSkills(std::unique_ptr<sql::ResultSet> result)
{
	while(result->next())
	{
		auto skill = std::make_shared<Skill>();

		std::string name = result->getString(1);
		skill->is_title = result->getBoolean(2);
		skill->is_profession = result->getBoolean(3);
		skill->money_required = result->getUInt(4);
		skill->skill_points_consumed = result->getUInt(5);
		skill->xp_type = result->getString(6);
		skill->xp_cost = result->getUInt(7);
		skill->xp_cap = result->getUInt(8);

		skills_.insert(std::make_pair(name, skill));
	}
}

void SkillManager::_loadSkillPrereqs(std::unique_ptr<sql::ResultSet> result)
{
	while(result->next())
	{
		std::string skill_name = result->getString(1);
		std::string prereq_name = result->getString(2);

		auto skill_find = skills_.find(skill_name);
		auto prereq_find = skills_.find(prereq_name);

		if(skill_find != skills_.end() && prereq_find != skills_.end())
		{
			skill_find->second->required_skills_.insert(prereq_find->second);
		}
	}
}

void SkillManager::_loadSkillsRequiredSpecies(std::unique_ptr<sql::ResultSet> results)
{
}

void SkillManager::_loadSkillMods(std::unique_ptr<sql::ResultSet> result)
{
	while(result->next())
	{
		std::string skillname = result->getString(1);
		auto find_itr = skills_.find(skillname);

		if(find_itr != skills_.end())
		{
			std::string modname = result->getString(2);
			find_itr->second->skill_mods_.insert(std::make_pair(modname, result->getUInt(3)));
		}
	}
}

std::pair<uint32_t, uint32_t> SkillManager::GetSkillMod(const std::shared_ptr<Creature>& creature, const std::string& skill_mod_name)
{
	std::map<std::string, std::pair<uint32_t, uint32_t>> mods = GetSkillMods(creature);

	auto find_itr = mods.find(skill_mod_name);
	
	if(find_itr != mods.end())
	{
		return find_itr->second;
	}

	return std::make_pair<uint32_t, uint32_t>(0, 0);
}

void addMod(std::map<std::string, std::pair<uint32_t, uint32_t>>& map, std::string modname, uint32_t baseDelta, uint32_t modDelta)
{
	auto lb = map.lower_bound(modname);

	if(lb != map.end() && !(map.key_comp()(modname, lb->first)))
	{
		lb->second.first += baseDelta;
		lb->second.second += modDelta;
	}
	else
	{
		map.insert(lb, std::make_pair(modname, std::make_pair(baseDelta, modDelta)));
	}
}

SkillModsMap SkillManager::GetSkillMods(const std::shared_ptr<Creature>& creature)
{
	std::map<std::string, std::pair<uint32_t, uint32_t>> result;

	//Add the base values
	auto skill_end = this->skills_.end();
	for(auto& skill : creature->GetSkills())
	{
		auto skill_itr = this->skills_.find(skill.name);
		if(skill_itr != skills_.end())
		{
			for(auto& mod : skill_itr->second->skill_mods_)
			{
				addMod(result, mod.first, mod.second, 0);
			}
		}
	}

	//Add the mod values
	creature->ViewObjects(creature, 1, true, [&] (std::shared_ptr<Object> child) {
		for(auto& mod : result)
		{
			auto modifier = child->GetAttributeRecursive(mod.first);
			if(modifier.which() == 1)
			{
				mod.second.second += boost::get<int32_t>(modifier);
			}
		}
	});

	return result;
}