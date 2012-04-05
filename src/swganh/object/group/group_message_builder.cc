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

#include "group_message_builder.h"

#include <swganh/object/group/group.h>
#include <swganh/object/tangible/tangible.h>

using namespace std;
using namespace swganh::object::group;
using namespace swganh::messages;

namespace swganh {
namespace object {
namespace group {

void GroupMessageBuilder::RegisterEventHandlers()
{
    // TODO: Register Handlers for Group
}

void GroupMessageBuilder::BuildMemberListDelta(shared_ptr<Group> group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 1);
        group->GetGroupMembers().Serialize(message);
        group->AddDeltasUpdate(move(message));
    }
    else
        group->GetGroupMembers().ClearDeltas();
}

void GroupMessageBuilder::BuildLootModeDelta(shared_ptr<Group> group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 7);
        message.data.write<uint32_t>(group->GetLootMode());
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildDifficultyDelta(shared_ptr<Group> group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 4);
        message.data.write<uint16_t>(group->GetDifficulty());
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildLootMasterDelta(shared_ptr<Group> group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 6);
        message.data.write<uint64_t>(group->GetLootMaster());
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildBaseline3(shared_ptr<Group> group)
{
    auto message = CreateBaselinesMessage(group, Object::VIEW_3, 8);
	ObjectMessageBuilder::BuildBaseline3(group);
}

void GroupMessageBuilder::BuildBaseline6(shared_ptr<Group> group)
{
    auto message = CreateBaselinesMessage(group, Object::VIEW_6, 6);
	message.data.append(ObjectMessageBuilder::BuildBaseline6(group).get().data);
    group->GetGroupMembers().Serialize(message);
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);
	message.data.write<std::string>("");
    message.data.write<uint16_t>(group->GetDifficulty());
	message.data.write<uint32_t>(4);
    message.data.write<uint64_t>(group->GetLootMaster());
    message.data.write<uint32_t>(group->GetLootMode());
	group->AddBaselineToCache(message);
}

}}} // swganh::object::group