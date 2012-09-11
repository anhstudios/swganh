// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/optional.hpp>
#include <cstdint>
#include "swganh_core/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {

    // Forward Declarations
    class Tangible;



class Guild;
class GuildMessageBuilder : public ObjectMessageBuilder
{
public:
    GuildMessageBuilder(swganh::EventDispatcher* dispatcher)
        : ObjectMessageBuilder(dispatcher)
    {
        RegisterEventHandlers();
    }
    virtual void RegisterEventHandlers();
    // deltas
    static void BuildGuildTagsDelta(const std::shared_ptr<Guild>& guild);

	virtual void SendBaselines(const std::shared_ptr<Guild>& guild, const std::shared_ptr<swganh::observer::ObserverInterface>& observer);

    // baselines
    static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Guild>& guild);
    static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Guild>& guild);
private:
	typedef swganh::ValueEvent<std::shared_ptr<Guild>> GuildEvent;
};

}} // swganh::object
