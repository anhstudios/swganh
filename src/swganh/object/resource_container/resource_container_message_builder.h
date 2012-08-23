// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_RESOURCE_CONTAINER_RESOURCE_CONTAINER_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_RESOURCE_CONTAINER_RESOURCE_CONTAINER_MESSAGE_BUILDER_H_

#include <cstdint>
#include <string>
#include <boost/optional.hpp>

#include "swganh/object/tangible/tangible_message_builder.h"

#include "resource_container.h"

namespace swganh {
namespace object {
namespace resource_container {

class ResourceContainerMessageBuilder : public swganh::object::tangible::TangibleMessageBuilder
{
public:
    ResourceContainerMessageBuilder(anh::EventDispatcher* event_dispatcher_)
        : TangibleMessageBuilder(event_dispatcher_) 
    {
        RegisterEventHandlers();
    }
    virtual void RegisterEventHandlers();
    virtual void SendBaselines(const std::shared_ptr<ResourceContainer>& resource_container, const std::shared_ptr<anh::observer::ObserverInterface>& observer);

    // deltas
    static void BuildCurrentUnitsDelta(const std::shared_ptr<ResourceContainer>& resource_container);
	static void BuildGlobalResourceIdDelta(const std::shared_ptr<ResourceContainer>& resource_container);
	static void BuildMaxQuantityDelta(const std::shared_ptr<ResourceContainer>& resource_container);
	static void BuildResourceTypeDelta(const std::shared_ptr<ResourceContainer>& resource_container);
	static void BuildVariationNameDelta(const std::shared_ptr<ResourceContainer>& resource_container);

    // baselines
    static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<ResourceContainer>& resource_container);
    static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<ResourceContainer>& resource_container);
private:
    typedef anh::ValueEvent<std::shared_ptr<ResourceContainer>> ResourceContainerEvent;
};

}}} // swganh::object::creature

#endif // SWGANH_OBJECT_CREATURE_CREATURE_MESSAGE_BUILDER_H_