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
#include <swganh/object/tangible/base_tangible.h>

using namespace std;
using namespace swganh::object::group;
using namespace swganh::messages;

namespace swganh {
namespace object {
namespace group {

void GroupMessageBuilder::BuildMemberListDelta(Group* group, uint8_t subtype, std::shared_ptr<swganh::object::tangible::BaseTangible> member)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 1);
        message.data.write<uint32_t>(group->member_list_.size());
        message.data.write<uint32_t>(group->member_list_counter_++);
        message.data.write<uint8_t>(subtype);

        auto iter = group->FindMemberIterById_(member->GetObjectId());
        switch(subtype)
        {
        case 0:
            {
                message.data.write<uint16_t>(iter - begin(group->member_list_));
                break;
            }

        case 1:
            {
                message.data.write<uint16_t>(iter - begin(group->member_list_));
                message.data.write<uint64_t>(iter->get()->GetObjectId());
                message.data.write<std::string>(std::string(iter->get()->GetCustomName().begin(), member->GetCustomName().end()));
                break;
            }
        default:
            {
                break;
            }
        }

        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildLootModeDelta(Group* group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 7);
        message.data.write<uint32_t>(group->loot_mode_);

        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildDifficultyDelta(Group* group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 4);
        message.data.write<uint16_t>(group->difficulty_);

        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildLootMasterDelta(Group* group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = group->CreateDeltasMessage(Object::VIEW_6, 6);
        message.data.write<uint64_t>(group->loot_master_);

        group->AddDeltasUpdate(move(message));
    }
}

boost::optional<BaselinesMessage> GroupMessageBuilder::BuildBaseline6(Group* group)
{
    auto message = group->CreateBaselinesMessage(Object::VIEW_6, 8);
	message.data.append(group->Object::GetBaseline6().get().data);
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(group->member_list_.size());
    message.data.write<uint32_t>(group->member_list_counter_++);
    std::for_each(group->member_list_.begin(), group->member_list_.end(), [=, &message](Group::MemberList::value_type list_member) {
		message.data.write<uint64_t>(list_member.get()->GetObjectId());
		message.data.write<std::string>(std::string(list_member.get()->GetCustomName().begin(), list_member.get()->GetCustomName().end()));
	});
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);
	message.data.write<std::string>("");
    message.data.write<uint16_t>(group->difficulty_);
	message.data.write<uint32_t>(0);
    message.data.write<uint64_t>(group->loot_master_);
    message.data.write<uint32_t>(group->loot_mode_);
	return boost::optional<BaselinesMessage>(std::move(message));
}

}}} // swganh::object::group