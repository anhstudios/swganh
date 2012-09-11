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
	static_object->AddBaselineToCache(&BuildBaseline3(static_object));
    static_object->AddBaselineToCache(&BuildBaseline6(static_object));

    for (auto& baseline : static_object->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
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