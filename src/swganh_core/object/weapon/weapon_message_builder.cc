// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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
    auto baseline3 = BuildBaseline3(weapon);
    auto baseline6 = BuildBaseline6(weapon);
    
    observer->Notify(&baseline3);
    observer->Notify(&baseline6);
        
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