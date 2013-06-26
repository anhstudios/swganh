// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "guild.h"

#include <sstream>

#include "guild_message_builder.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;

Guild::Guild()
    : Object()
{
    SetTemplate("object/guild/shared_guild_object.iff");
}

Guild::~Guild()
{
}

void Guild::AddGuildTag(uint32_t guild_id, std::string guild_tag) {
    auto lock = AcquireLock();
    AddGuildTag(guild_id, guild_tag, lock);
}

void Guild::AddGuildTag(uint32_t guild_id, std::string guild_tag, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter == end(guild_list_))
    {
		guild_list_.add(GuildTag(guild_id, guild_tag));
		DISPATCH(Guild, Tag);
	}
}

void Guild::RemoveGuildTag(uint32_t guild_id) {
    auto lock = AcquireLock();
    RemoveGuildTag(guild_id, lock);
}

void Guild::RemoveGuildTag(uint32_t guild_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter != end(guild_list_))
    {
		guild_list_.remove(iter);
		DISPATCH(Guild, Tag);
	}
}
    
std::set<GuildTag> Guild::GetGuildList() {
    auto lock = AcquireLock();
    return GetGuildList(lock);
}

std::set<GuildTag> Guild::GetGuildList(boost::unique_lock<boost::mutex>& lock)
{
    return guild_list_.raw();
}

void Guild::SerializeGuildList(swganh::messages::BaseSwgMessage* message) {
    auto lock = AcquireLock();
    SerializeGuildList(message, lock);
}

void Guild::SerializeGuildList(swganh::messages::BaseSwgMessage* message, boost::unique_lock<boost::mutex>& lock)
{
	guild_list_.Serialize(message);
}
