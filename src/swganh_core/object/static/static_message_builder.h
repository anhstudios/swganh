// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_STATIC_STATIC_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_STATIC_STATIC_MESSAGE_BUILDER_H_

#include <boost/optional.hpp>
#include <cstdint>
#include "swganh_core/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {


class Static;
class StaticMessageBuilder : public ObjectMessageBuilder
{
public:
    StaticMessageBuilder(swganh::EventDispatcher* dispatcher)
        : ObjectMessageBuilder(dispatcher)
    {
        RegisterEventHandlers();
    }

    // baselines
    static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(const std::shared_ptr<Static>& static_object, boost::unique_lock<boost::mutex>& lock);
    static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(const std::shared_ptr<Static>& static_object, boost::unique_lock<boost::mutex>& lock);

private:
	typedef swganh::ValueEvent<std::shared_ptr<Static>> StaticEvent;
    
    void RegisterEventHandlers();
};

}} // swganh::object

#endif // SWGANH_OBJECT_GUILD_GUILD_MESSAGE_BUILDER_H_