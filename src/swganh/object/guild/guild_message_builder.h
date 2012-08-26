// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/optional.hpp>
#include <cstdint>
#include "swganh/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {

    // Forward Declarations
    class Tangible;

namespace guild {

class Guild;
class GuildMessageBuilder : public ObjectMessageBuilder
{
public:
    GuildMessageBuilder(anh::EventDispatcher* dispatcher)
        : ObjectMessageBuilder(dispatcher)
    {
        RegisterEventHandlers();
    }
    virtual void RegisterEventHandlers();
    // deltas
    static void BuildGuildTagsDelta(const std::shared_ptr<Guild>& guild);

    // baselines
    static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Guild>& guild);
    static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Guild>& guild);
};

}}} // swganh::object::guild
