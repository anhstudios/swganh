// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <set>


namespace swganh
{
namespace observer
{
	class ObserverInterface;
}

namespace chat
{

struct Room
{
	uint32_t id_;
	bool is_private_, is_muted_;
	std::string name_, title_;
	uint64_t owner_, creator_;
	std::set<uint64_t> moderators_, invited_users_, banned_users_;
	std::set<std::shared_ptr<swganh::observer::ObserverInterface>> users_;
};

}
}