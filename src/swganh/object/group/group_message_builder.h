// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <boost/optional.hpp>
#include <cstdint>
#include "swganh/object/object_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace tangible {
    class Tangible;
}}} // swganh::object::tangible

namespace swganh {
namespace object {
namespace group {

class Group;
class GroupMessageBuilder : public ObjectMessageBuilder
{
public:
    GroupMessageBuilder(anh::EventDispatcher* event_dispatcher)
        : ObjectMessageBuilder(event_dispatcher)
    {
        RegisterEventHandlers();
    }
    // deltas
    virtual void RegisterEventHandlers();
    static void BuildMemberListDelta(const std::shared_ptr<Group>& group);
    static void BuildLootModeDelta(const std::shared_ptr<Group>& group);
    static void BuildDifficultyDelta(const std::shared_ptr<Group>& group);
    static void BuildLootMasterDelta(const std::shared_ptr<Group>& group);

    // baselines
    static void BuildBaseline3(const std::shared_ptr<Group>& group);
    static void BuildBaseline6(const std::shared_ptr<Group>& group);
};

}}} // swganh::object::group
