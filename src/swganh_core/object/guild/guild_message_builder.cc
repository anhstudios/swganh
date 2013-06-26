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
        guild->SerializeGuildList(&message);
        guild->AddDeltasUpdate(&message);
    }
}

boost::optional<BaselinesMessage> GuildMessageBuilder::BuildBaseline3(const shared_ptr<Guild>& guild, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(guild, lock, Object::VIEW_3, 5);
    message.data.append((*ObjectMessageBuilder::BuildBaseline3(guild, lock)).data);
    guild->SerializeGuildList(&message, lock);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> GuildMessageBuilder::BuildBaseline6(const shared_ptr<Guild>& guild, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(guild, lock, Object::VIEW_6, 5);
    message.data.append((*ObjectMessageBuilder::BuildBaseline6(guild, lock)).data);
    return BaselinesMessage(std::move(message));
}
