/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_OBJECT_GROUP_GROUP_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_GROUP_GROUP_MESSAGE_BUIlDER_H_

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
    static void BuildMemberListDelta(std::shared_ptr<Group> group);
    static void BuildLootModeDelta(std::shared_ptr<Group> group);
    static void BuildDifficultyDelta(std::shared_ptr<Group> group);
    static void BuildLootMasterDelta(std::shared_ptr<Group> group);

    // baselines
    static void BuildBaseline3(Group* group);
    static void BuildBaseline6(Group* group);
};

}}} // swganh::object::group

#endif // SWGANH_OBJECT_GROUP_GROUP_MESSAGE_BUILDER_H_