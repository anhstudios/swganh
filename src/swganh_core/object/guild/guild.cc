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

void Guild::AddGuildTag(uint32_t guild_id, std::string guild_tag)
{
    boost::lock_guard<boost::mutex> lk(object_mutex_);
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter != end(guild_list_))
    {
        return;
    }

    guild_list_.Add(GuildTag(guild_id, guild_tag));
        
    GetEventDispatcher()->Dispatch(make_shared<GuildEvent>
        ("Guild::Orientation",static_pointer_cast<Guild>(shared_from_this())));
}

void Guild::RemoveGuildTag(uint32_t guild_id)
{
    boost::lock_guard<boost::mutex> lk(object_mutex_);
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter != end(guild_list_))
    {
        return;
    }

    guild_list_.Remove(iter);
    
    GetEventDispatcher()->Dispatch(make_shared<GuildEvent>
        ("Guild::Orientation",static_pointer_cast<Guild>(shared_from_this())));
}
    
swganh::messages::containers::NetworkList<GuildTag> Guild::GetGuildList()
{
    boost::lock_guard<boost::mutex> lk(object_mutex_);
    return guild_list_;
}