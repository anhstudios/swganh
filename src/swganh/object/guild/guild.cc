
#include "swganh/object/guild/guild.h"

#include <sstream>

#include <swganh/object/guild/guild_message_builder.h>

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::guild;

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
    {
        std::lock_guard<std::mutex> lk(guild_mutex_);
        auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
            return guild_id == tag.id;
        });

        if(iter != end(guild_list_))
        {
            return;
        }

        guild_list_.Add(GuildTag(guild_id, guild_tag));
    }
        
    GuildMessageBuilder::BuildGuildTagsDelta(this);
}

void Guild::RemoveGuildTag(uint32_t guild_id)
{
    {
        std::lock_guard<std::mutex> lk(guild_mutex_);
        auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
            return guild_id == tag.id;
        });

        if(iter != end(guild_list_))
        {
            return;
        }

        guild_list_.Remove(iter);
    }

    GuildMessageBuilder::BuildGuildTagsDelta(this);
}
    
swganh::messages::containers::NetworkList<GuildTag> Guild::GetGuildList()
{
    std::lock_guard<std::mutex> lk(guild_mutex_);
    return guild_list_;
}

boost::optional<BaselinesMessage> Guild::GetBaseline3()
{
    return GuildMessageBuilder::BuildBaseline3(this);
}

boost::optional<BaselinesMessage> Guild::GetBaseline6()
{
    return GuildMessageBuilder::BuildBaseline6(this);
}