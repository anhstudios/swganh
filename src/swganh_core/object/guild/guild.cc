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

void Guild::AddGuildTag(uint32_t guild_id, std::string guild_tag) { AddGuildTag(guild_id, guild_tag, AcquireLock()); }
void Guild::AddGuildTag(uint32_t guild_id, std::string guild_tag, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter != end(guild_list_))
    {
        return;
    }

    guild_list_.Add(GuildTag(guild_id, guild_tag));
    DISPATCH(Guild, Tag);
}

void Guild::RemoveGuildTag(uint32_t guild_id) { RemoveGuildTag(guild_id, AcquireLock()); }
void Guild::RemoveGuildTag(uint32_t guild_id, boost::unique_lock<boost::mutex>& lock)
{
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter != end(guild_list_))
    {
        return;
    }

    guild_list_.Remove(iter);
    DISPATCH(Guild, Tag);
}
    
swganh::messages::containers::NetworkList<GuildTag>& Guild::GetGuildList() { return GetGuildList(AcquireLock()); }
swganh::messages::containers::NetworkList<GuildTag>& Guild::GetGuildList(boost::unique_lock<boost::mutex>& lock)
{
    return guild_list_;
}