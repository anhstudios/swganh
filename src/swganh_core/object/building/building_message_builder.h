// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_BUILDING_BUILDING_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_BUILDING_BUILDING_MESSAGE_BUILDER_H_

#include "swganh_core/object/tangible/tangible_message_builder.h"

namespace swganh {
namespace object {


    class Building;
    class BuildingMessageBuilder : public swganh::object::TangibleMessageBuilder
    {
    public:
        BuildingMessageBuilder(swganh::EventDispatcher* dispatcher) :
            TangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();

        // baselines
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(const std::shared_ptr<Building>& building, boost::unique_lock<boost::mutex>& lock);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(const std::shared_ptr<Building>& building, boost::unique_lock<boost::mutex>& lock);
    private:
        typedef swganh::ValueEvent<std::shared_ptr<Building>> BuildingEvent;
    };

}}  // swganh::object

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_
