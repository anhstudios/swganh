
#include "swganh/object/guild/guild.h"

#include <sstream>

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
        guild_list_.insert(std::pair<uint32_t, std::string>(guild_id, guild_tag));

        if(HasObservers())
        {
            DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
            message.data.write<uint16_t>(4);
            message.data.write<uint32_t>(guild_list_.size());
            message.data.write<uint32_t>(guild_list_counter_++);
            message.data.write<uint8_t>(1);
            std::stringstream guild_id_tag;
            guild_id_tag << guild_id << ":" << guild_tag;
            message.data.write<std::string>(guild_id_tag.str());

            AddDeltasUpdate(move(message));
        }
    }
}

void Guild::RemoveGuild(uint32_t guild_id)
{
    auto iter = guild_list_.find(guild_id);
    if(iter != guild_list_.end())
    {
        guild_list_.erase(iter);
        
        if(HasObservers())
        {
            DeltasMessage message = CreateDeltasMessage(Object::VIEW_3);
            message.data.write<uint16_t>(4);
            message.data.write<uint32_t>(guild_list_.size());
            message.data.write<uint32_t>(guild_list_counter_++);
            message.data.write<uint8_t>(0);
            std::stringstream guild_id_tag;
            guild_id_tag << guild_id << ":" << iter->second;
            message.data.write<std::string>(guild_id_tag.str());

            AddDeltasUpdate(move(message));
        }
    }
}

boost::optional<BaselinesMessage> Guild::GetBaseline3()
{
    auto message = CreateBaselinesMessage(Object::VIEW_3, 5);
    message.data.append(Object::GetBaseline3().get().data);
    message.data.write<uint32_t>(guild_list_.size());
    message.data.write<uint32_t>(guild_list_counter_);
    std::for_each(guild_list_.begin(), guild_list_.end(), [=, &message](std::pair<uint32_t, std::string> guild) {
        std::stringstream guild_id_tag;
        guild_id_tag << guild.first << ":" << guild.second;
        // Write each guild as ID:TAG (i.e. "1234:DARK")
        message.data.write<std::string>(guild_id_tag.str());
    });

    return message;
}

