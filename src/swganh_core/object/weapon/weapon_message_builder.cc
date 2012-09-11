#include "weapon_message_builder.h"

#include "weapon.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

void WeaponMessageBuilder::RegisterEventHandlers()
{
}

void WeaponMessageBuilder::SendBaselines(const shared_ptr<Weapon>& weapon, const shared_ptr<swganh::observer::ObserverInterface>& observer)
{
    weapon->AddBaselineToCache(&BuildBaseline3(weapon));
    weapon->AddBaselineToCache(&BuildBaseline6(weapon));

    for (auto& baseline : weapon->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
    SendEndBaselines(weapon, observer);
}

// baselines
BaselinesMessage WeaponMessageBuilder::BuildBaseline3(const shared_ptr<Weapon>& weapon)
{
    auto message = CreateBaselinesMessage(weapon, Object::VIEW_3, 11);
    message.data.append(TangibleMessageBuilder::BuildBaseline3(weapon).data);
    return BaselinesMessage(std::move(message));
}

BaselinesMessage WeaponMessageBuilder::BuildBaseline6(const shared_ptr<Weapon>& weapon)
{
    auto message = CreateBaselinesMessage(weapon, Object::VIEW_6, 2);
    message.data.append(TangibleMessageBuilder::BuildBaseline6(weapon).data);
 
    return BaselinesMessage(std::move(message));
}