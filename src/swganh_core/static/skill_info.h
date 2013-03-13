// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace statics
{

struct SkillInfo
{
	bool is_title;
	bool is_profession;
	uint32_t money_required;
	uint32_t skill_points_consumed;

	std::string xp_type;
	uint32_t xp_cost;
	uint32_t xp_cap;

	std::set<std::shared_ptr<SkillInfo>> required_skills_;
	std::set<std::string> commands_;
	std::set<std::string> required_species_;
	
	std::map<std::string, uint32_t> skill_mods_;
};

}
}