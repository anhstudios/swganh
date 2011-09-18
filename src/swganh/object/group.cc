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

#include "group.h"

#include <swganh/object/tangible.h>
#include <swganh/scene/messages/deltas_message.h>
#include <swganh/scene/scene.h>

using namespace swganh::scene::messages;

namespace swganh {
namespace object {
namespace universe {

Group::Group()
    : member_list_counter_(0)
    , group_level_(0)
    , master_looter_(0)
    , loot_mode_(FREE_LOOT)
    , last_member_index_(0)
{
}

Group::~Group()
{
}

void Group::AddGroupMember(std::shared_ptr<Tangible> member)
{
    uint16_t next_index = GetNextMemberIndex_();
    member_list_.insert(std::make_pair(next_index, member));
    last_member_index_ = next_index;

    if(GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        message.data.write<uint16_t>(1);
        message.data.write<uint32_t>(member_list_.size());
        message.data.write<uint32_t>(member_list_counter_++);
        message.data.write<uint8_t>(1);
        message.data.write<uint16_t>(next_index);
        message.data.write<uint64_t>(member->GetObjectId());
        message.data.write<std::string>(std::string(member->GetCustomName().begin(), member->GetCustomName().end()));

        GetScene()->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_6, std::move(message)));
    }
}

void Group::RemoveGroupMember(std::shared_ptr<Tangible> member)
{
    uint16_t index_position = 0;
    auto iter = std::find_if(member_list_.begin(), member_list_.end(), [=, &index_position](std::shared_ptr<Tangible> list_member) {
        if(member->GetObjectId() == list_member->GetObjectId())
        {
            return true;
        }

        index_position++;
        return false;
    });

    if(iter != member_list_.end())
    {
        member_list_.erase(iter);

        if(GetScene()->HasObservers(GetObjectId()))
        {
            DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
            message.data.write<uint16_t>(1);
            message.data.write<uint32_t>(member_list_.size());
            message.data.write<uint32_t>(member_list_counter_++);
            message.data.write<uint8_t>(0);
            message.data.write<uint16_t>(index_position);
        }

        member_index_free_list_.push(index_position);
    }
}

void Group::SetLootMode(LOOT_MODE loot_mode)
{
    loot_mode_ = loot_mode;
        
    if(GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        message.data.write<uint16_t>(7); // Update Type
        message.data.write<uint32_t>(loot_mode_);
        
        GetScene()->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_6, std::move(message)));
    }
}

void Group::SetGroupLevel(uint16_t group_level)
{
    group_level_ = group_level;

    if(GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        message.data.write<uint16_t>(4); // Update Type
        message.data.write<uint16_t>(group_level_);

        GetScene()->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_6, std::move(message)));
    }
}

void Group::SetMasterLooter(uint64_t master_looter)
{
    master_looter_ = master_looter;
        
    if(GetScene()->HasObservers(GetObjectId()))
    {
        DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_6);
        message.data.write<uint16_t>(6);
        message.data.write<uint64_t>(master_looter_);

        GetScene()->UpdateObservers(GetObjectId(), message);
        delta_cache_.push_back(std::make_pair(BaseObject::VIEW_6, std::move(message)));
    }
}


uint16_t Group::GetNextMemberIndex_()
{
    uint16_t next_member_index = last_member_index_++;

    if(member_index_free_list_.size() > 0) {
        next_member_index = member_index_free_list_.top();
        member_index_free_list_.pop();
    }

    return next_member_index;
}


}}} // namespace