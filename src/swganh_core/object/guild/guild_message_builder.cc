// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "guild_message_builder.h"

#include <sstream>

#include "swganh_core/object/guild/guild.h"

using namespace std;
using namespace swganh::object;
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
        guild->AddDeltasUpdate(&message);
    }
    else
        guild->GetGuildList().ClearDeltas();
}

void GuildMessageBuilder::SendBaselines(const std::shared_ptr<Guild>& guild, const std::shared_ptr<swganh::observer::ObserverInterface>& observer)
{
	guild->AddBaselineToCache(&BuildBaseline3(guild));
    guild->AddBaselineToCache(&BuildBaseline6(guild));

    for (auto& baseline : guild->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
    SendEndBaselines(guild, observer);
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