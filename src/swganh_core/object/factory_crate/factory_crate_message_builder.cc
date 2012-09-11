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

void FactoryCrateMessageBuilder::SendBaselines(const shared_ptr<FactoryCrate>& factory_crate, const shared_ptr<swganh::observer::ObserverInterface>& observer)
{
    factory_crate->AddBaselineToCache(&BuildBaseline3(factory_crate));
    factory_crate->AddBaselineToCache(&BuildBaseline6(factory_crate));

    for (auto& baseline : factory_crate->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
    SendEndBaselines(factory_crate, observer);
}

// baselines
BaselinesMessage FactoryCrateMessageBuilder::BuildBaseline3(const shared_ptr<FactoryCrate>& factory_crate)
{
    auto message = CreateBaselinesMessage(factory_crate, Object::VIEW_3, 11);
    message.data.append(TangibleMessageBuilder::BuildBaseline3(factory_crate).data);
    return BaselinesMessage(std::move(message));
}

BaselinesMessage FactoryCrateMessageBuilder::BuildBaseline6(const shared_ptr<FactoryCrate>& factory_crate)
{
    auto message = CreateBaselinesMessage(factory_crate, Object::VIEW_6, 2);
    message.data.append(TangibleMessageBuilder::BuildBaseline6(factory_crate).data);
 
    return BaselinesMessage(std::move(message));
}