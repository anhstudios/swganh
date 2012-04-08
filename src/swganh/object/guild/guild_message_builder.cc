#include "guild_message_builder.h"

#include <sstream>

#include <swganh/object/guild/guild.h>

using namespace std;
using namespace swganh::object::guild;
using namespace swganh::messages;

void GuildMessageBuilder::RegisterEventHandlers()
{
    // TODO: Register guild event handlers
}

void GuildMessageBuilder::BuildGuildTagsDelta(const shared_ptr<Guild>& guild)
{
    if(guild->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(guild, Object::VIEW_3, 4);
        guild->GetGuildList().Serialize(message);
        guild->AddDeltasUpdate(std::move(message));
    }
    else
        guild->GetGuildList().ClearDeltas();
}

BaselinesMessage GuildMessageBuilder::BuildBaseline3(const shared_ptr<Guild>& guild)
{
    auto message = CreateBaselinesMessage(guild, Object::VIEW_3, 5);
    message.data.append(ObjectMessageBuilder::BuildBaseline3(guild).data);
    guild->GetGuildList().Serialize(message);
    return BaselinesMessage(std::move(message));
}

BaselinesMessage GuildMessageBuilder::BuildBaseline6(const shared_ptr<Guild>& guild)
{
    auto message = CreateBaselinesMessage(guild, Object::VIEW_6, 5);
    message.data.append(ObjectMessageBuilder::BuildBaseline6(guild).data);
    return BaselinesMessage(std::move(message));
}