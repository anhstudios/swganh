// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_WEAPON_WEAPON_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_WEAPON_WEAPON_MESSAGE_BUILDER_H_

#include "swganh_core/object/tangible/tangible_message_builder.h"

namespace swganh {
namespace object {


    class Weapon;
    class WeaponMessageBuilder : public swganh::object::TangibleMessageBuilder
    {
    public:
        WeaponMessageBuilder(swganh::EventDispatcher* dispatcher) :
            TangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }

        // baselines
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(const std::shared_ptr<Weapon>& weapon, boost::unique_lock<boost::mutex>& lock);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(const std::shared_ptr<Weapon>& weapon, boost::unique_lock<boost::mutex>& lock);

    private:
        typedef swganh::ValueEvent<std::shared_ptr<Weapon>> WeaponEvent;
        
        void RegisterEventHandlers();
    };

}}  // swganh::object

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_
