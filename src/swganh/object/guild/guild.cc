
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
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter == end(guild_list_))
    {
        guild_list_.Add(GuildTag(guild_id, guild_tag));
        GuildMessageBuilder::BuildGuildTagsDelta(this);
    }
}

void Guild::RemoveGuildTag(uint32_t guild_id)
{
    auto iter = std::find_if(begin(guild_list_), end(guild_list_), [=](const GuildTag& tag)->bool {
        return guild_id == tag.id;
    });

    if(iter != end(guild_list_))
    {
        guild_list_.Remove(iter);
        GuildMessageBuilder::BuildGuildTagsDelta(this);
    }
}

boost::optional<BaselinesMessage> Guild::GetBaseline3()
{
    return GuildMessageBuilder::BuildBaseline3(this);
}

boost::optional<BaselinesMessage> Guild::GetBaseline6()
{
    return GuildMessageBuilder::BuildBaseline6(this);
}