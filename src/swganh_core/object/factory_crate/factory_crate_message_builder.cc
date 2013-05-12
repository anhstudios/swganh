// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "factory_crate_message_builder.h"

#include "factory_crate.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

void FactoryCrateMessageBuilder::RegisterEventHandlers()
{
}

// baselines
boost::optional<BaselinesMessage> FactoryCrateMessageBuilder::BuildBaseline3(const shared_ptr<FactoryCrate>& factory_crate, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(factory_crate, lock, Object::VIEW_3, 11);
    message.data.append((*TangibleMessageBuilder::BuildBaseline3(factory_crate, lock)).data);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> FactoryCrateMessageBuilder::BuildBaseline6(const shared_ptr<FactoryCrate>& factory_crate, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(factory_crate, lock, Object::VIEW_6, 2);
    message.data.append((*TangibleMessageBuilder::BuildBaseline6(factory_crate, lock)).data);
 
    return BaselinesMessage(std::move(message));
}