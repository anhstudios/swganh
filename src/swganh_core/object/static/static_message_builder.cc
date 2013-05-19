// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "static_message_builder.h"

#include "static.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

void StaticMessageBuilder::RegisterEventHandlers()
{
}

// baselines
boost::optional<BaselinesMessage> StaticMessageBuilder::BuildBaseline3(const std::shared_ptr<Static>& static_object, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(static_object, lock, Object::VIEW_3, 5);
    message.data.append((*ObjectMessageBuilder::BuildBaseline3(static_object, lock)).data);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> StaticMessageBuilder::BuildBaseline6(const std::shared_ptr<Static>& static_object, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(static_object, lock, Object::VIEW_6, 5);
    message.data.append((*ObjectMessageBuilder::BuildBaseline6(static_object, lock)).data);
    return BaselinesMessage(std::move(message));
}
