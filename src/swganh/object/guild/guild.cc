
#include "swganh/object/guild/guild.h"

#include <sstream>

#include <swganh/object/guild/guild_message_builder.h>

using namespace std;
using namespace swganh::messages;
using namespace swganh::object::guild;

Guild::Guild()
    : guild_list_counter_(0)
{
}

Guild::Guild(std::map<uint32_t, std::string> guild_list)
    : guild_list_(guild_list)
    , guild_list_counter_(0)
{
}

Guild::~Guild()
{
}

void Guild::AddGuild(uint32_t guild_id, std::string guild_tag)
{
    auto iter = guild_list_.find(guild_id);
    if(iter == guild_list_.end())
    {
        std::stringstream guild_id_tag;
        guild_id_tag << guild_id << ":" << guild_tag;

        guild_list_.insert(std::pair<uint32_t, std::string>(guild_id, guild_tag));
       
        GuildMessageBuilder::BuildGuildListDelta(this, 1, guild_id_tag.str());
        IncrementGuildsListCounter();
    }
}

void Guild::RemoveGuild(uint32_t guild_id)
{
    auto iter = guild_list_.find(guild_id);
    if(iter != guild_list_.end())
    {
        std::stringstream guild_id_tag;
        guild_id_tag << iter->first << ":" << iter->second;

        guild_list_.erase(iter);
        
        GuildMessageBuilder::BuildGuildListDelta(this, 1, guild_id_tag.str());
        IncrementGuildsListCounter();
    }
}

boost::optional<BaselinesMessage> Guild::GetBaseline3()
{
    return GuildMessageBuilder::BuildBaseline3(this);
}