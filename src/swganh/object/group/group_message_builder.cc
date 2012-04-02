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

void GroupMessageBuilder::BuildMemberListDelta(Group* group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 1);
        group->GetGroupMembers().Serialize(message);
        group->AddDeltasUpdate(move(message));
    }
    else
        group->GetGroupMembers().ClearDeltas();
}

void GroupMessageBuilder::BuildLootModeDelta(Group* group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 7);
        message.data.write<uint32_t>(group->GetLootMode());
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildDifficultyDelta(Group* group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 4);
        message.data.write<uint16_t>(group->GetDifficulty());
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildLootMasterDelta(Group* group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 6);
        message.data.write<uint64_t>(group->GetLootMaster());
        group->AddDeltasUpdate(move(message));
    }
}

boost::optional<BaselinesMessage> GroupMessageBuilder::BuildBaseline3(Group* group)
{
    auto message = group->CreateBaselinesMessage(Object::VIEW_3, 8);
	message.data.append(group->Object::GetBaseline3().get().data);
    return boost::optional<BaselinesMessage>(std::move(message));
}

boost::optional<BaselinesMessage> GroupMessageBuilder::BuildBaseline6(Group* group)
{
    auto message = group->CreateBaselinesMessage(Object::VIEW_6, 6);
	message.data.append(group->Object::GetBaseline6().get().data);
    group->GetGroupMembers().Serialize(message);
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);
	message.data.write<std::string>("");
    message.data.write<uint16_t>(group->GetDifficulty());
	message.data.write<uint32_t>(4);
    message.data.write<uint64_t>(group->GetLootMaster());
    message.data.write<uint32_t>(group->GetLootMode());
	return boost::optional<BaselinesMessage>(std::move(message));
}

}}} // swganh::object::group