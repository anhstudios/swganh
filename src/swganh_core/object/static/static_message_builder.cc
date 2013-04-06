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

void StaticMessageBuilder::SendBaselines(const std::shared_ptr<Static>& static_object, const std::shared_ptr<swganh::observer::ObserverInterface>& observer)
{
    auto baseline3 = BuildBaseline3(static_object);
    auto baseline6 = BuildBaseline6(static_object);
    
	observer->Notify(&baseline3);
    observer->Notify(&baseline6);

    SendEndBaselines(static_object, observer);
}

// baselines
BaselinesMessage StaticMessageBuilder::BuildBaseline3(const std::shared_ptr<Static>& static_object)
{
	auto message = CreateBaselinesMessage(static_object, Object::VIEW_3, 5);
    message.data.append(ObjectMessageBuilder::BuildBaseline3(static_object).data);
    return BaselinesMessage(std::move(message));
}

BaselinesMessage StaticMessageBuilder::BuildBaseline6(const std::shared_ptr<Static>& static_object)
{
	auto message = CreateBaselinesMessage(static_object, Object::VIEW_6, 5);
    message.data.append(ObjectMessageBuilder::BuildBaseline6(static_object).data);
    return BaselinesMessage(std::move(message));
}