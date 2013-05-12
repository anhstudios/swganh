// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "building_message_builder.h"

#include "building.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

void BuildingMessageBuilder::RegisterEventHandlers()
{
}

// baselines
boost::optional<BaselinesMessage> BuildingMessageBuilder::BuildBaseline3(const shared_ptr<Building>& building, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(building, lock, Object::VIEW_3, 11);
    message.data.append((*TangibleMessageBuilder::BuildBaseline3(building, lock)).data);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> BuildingMessageBuilder::BuildBaseline6(const shared_ptr<Building>& building, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(building, lock, Object::VIEW_6, 2);
    message.data.append((*TangibleMessageBuilder::BuildBaseline6(building, lock)).data);
    return BaselinesMessage(std::move(message));
}