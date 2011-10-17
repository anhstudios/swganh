#include "guild_message_builder.h"

#include <sstream>

#include <swganh/object/guild/guild.h>

using namespace std;
using namespace swganh::object::guild;
using namespace swganh::messages;

void GuildMessageBuilder::BuildGuildListDelta(Guild* guild, uint8_t subtype, std::string guild_id_tag)
{
    if(guild->HasObservers())
    {
        DeltasMessage message = guild->CreateDeltasMessage(Object::VIEW_3, 4);
        message.data.write<uint32_t>(guild->guild_list_.size());
        message.data.write<uint32_t>(guild->guild_list_counter_++);
        message.data.write<uint8_t>(subtype);

        switch(subtype)
        {
        case 0: // Remove
            {
                message.data.write<std::string>(guild_id_tag);
                break;
            }
        case 1: // Add
            {
                message.data.write<std::string>(guild_id_tag);
                break;
            }
        default:
            {
                break;
            }
        }

        guild->AddDeltasUpdate(std::move(message));
    }
}

boost::optional<BaselinesMessage> GuildMessageBuilder::BuildBaseline3(Guild* guild)
{
    auto message = guild->CreateBaselinesMessage(Object::VIEW_3, 5);
    message.data.append(guild->Object::GetBaseline3().get().data);
    message.data.write<uint32_t>(guild->guild_list_.size());
    message.data.write<uint32_t>(guild->guild_list_counter_++);
    // Write each guild as ID:TAG (i.e. "1234:DARK")
    std::for_each(guild->guild_list_.begin(), guild->guild_list_.end(), [=, &message](std::pair<uint32_t, std::string> guild) {
        std::stringstream guild_id_tag;
        guild_id_tag << guild.first << ":" << guild.second;
        message.data.write<std::string>(guild_id_tag.str());
    });

    return message;
}