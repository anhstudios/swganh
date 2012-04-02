#include "guild_message_builder.h"

#include <sstream>

#include <swganh/object/guild/guild.h>

using namespace std;
using namespace swganh::object::guild;
using namespace swganh::messages;

void GuildMessageBuilder::BuildGuildTagsDelta(Guild* guild)
{
    if(guild->HasObservers())
    {
        DeltasMessage message = guild->CreateDeltasMessage(Object::VIEW_3, 4);
        guild->GetGuildList().Serialize(message);
        guild->AddDeltasUpdate(std::move(message));
    }
    else
        guild->GetGuildList().ClearDeltas();
}

boost::optional<BaselinesMessage> GuildMessageBuilder::BuildBaseline3(Guild* guild)
{
    auto message = guild->CreateBaselinesMessage(Object::VIEW_3, 5);
    message.data.append(guild->Object::GetBaseline3().get().data);
    guild->GetGuildList().Serialize(message);
    return boost::optional<BaselinesMessage>(std::move(message));
}

boost::optional<BaselinesMessage> GuildMessageBuilder::BuildBaseline6(Guild* guild)
{
    auto message = guild->CreateBaselinesMessage(Object::VIEW_6, 5);
    message.data.append(guild->Object::GetBaseline6().get().data);
    return boost::optional<BaselinesMessage>(std::move(message));
}