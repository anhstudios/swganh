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

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace tangible {
    class BaseTangible;
}}} // swganh::object::tangible

namespace swganh {
namespace object {
namespace group {

class Group;
class GroupMessageBuilder
{
public:

    // deltas
    static void BuildMemberListDelta(Group* group, uint8_t subtype, uint16_t index, std::shared_ptr<swganh::object::tangible::BaseTangible> member);
    static void BuildLootModeDelta(Group* group);
    static void BuildDifficultyDelta(Group* group);
    static void BuildLootMasterDelta(Group* group);

    // baselines
    static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(Group* group);
};

}}} // swganh::object::group

#endif // SWGANH_OBJECT_GROUP_GROUP_MESSAGE_BUILDER_H_