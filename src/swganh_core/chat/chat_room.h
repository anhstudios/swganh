// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{
	class Creature;
}

namespace chat
{

struct Member
{
	bool is_present, is_moderator, is_invited;

	std::shared_ptr<swganh::object::ObjectController> controller_;

	std::string custom_name;
	
};

struct ChatRoom
{
	std::string name, title, creator_name, owner_name;
	uint64_t creator_id, owner_id;
	
	bool is_private, is_muted;
	
	std::map<uint64_t, Member> members_;
	std::set<uint64_t> bans_;
};

}
}