// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/optional.hpp>
#include <cstdint>
#include "swganh_core/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {

    class Tangible;
}} // swganh::object

namespace swganh {
namespace object {


class Group;
class GroupMessageBuilder : public ObjectMessageBuilder
{
public:
    GroupMessageBuilder(swganh::EventDispatcher* event_dispatcher)
        : ObjectMessageBuilder(event_dispatcher)
    {
        RegisterEventHandlers();
    }

    // deltas
    static void BuildMemberListDelta(const std::shared_ptr<Group>& group);
    static void BuildLootModeDelta(const std::shared_ptr<Group>& group);
    static void BuildDifficultyDelta(const std::shared_ptr<Group>& group);
    static void BuildLootMasterDelta(const std::shared_ptr<Group>& group);

	static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(const std::shared_ptr<Group>& group, boost::unique_lock<boost::mutex>& lock);
    static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(const std::shared_ptr<Group>& group, boost::unique_lock<boost::mutex>& lock);

private:
	typedef swganh::ValueEvent<std::shared_ptr<Group>> GroupEvent;
    
    void RegisterEventHandlers();
};

}} // swganh::object
