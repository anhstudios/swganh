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

#include "guild.h"

#include <sstream>
#include <swganh/scene/messages/deltas_message.h>
#include <swganh/scene/scene.h>

using namespace swganh::scene::messages;

namespace swganh {
namespace object {

Guild::Guild()
    : guild_list_counter_(0)
{
}

Guild::Guild(std::map<uint32_t, std::string> guild_list)
    : guild_list_(guild_list)
    , guild_list_counter_(0)
{
}

Guild::~Guild()
{
}

void Guild::AddGuild(uint32_t guild_id, std::string guild_tag)
{
    auto iter = guild_list_.find(guild_id);
    if(iter == guild_list_.end())
    {
        guild_list_.insert(std::pair<uint32_t, std::string>(guild_id, guild_tag));

        if(GetScene()->HasObservers(GetObjectId()))
        {
            DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
            message.data.write<uint16_t>(4);
            message.data.write<uint32_t>(guild_list_.size());
            message.data.write<uint32_t>(guild_list_counter_++);
            message.data.write<uint8_t>(1);
            std::stringstream guild_id_tag;
            guild_id_tag << guild_id << ":" << guild_tag;
            message.data.write<std::string>(guild_id_tag.str());
            GetScene()->UpdateObservers(GetObjectId(), message);
            deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
        }
    }
}

void Guild::RemoveGuild(uint32_t guild_id)
{
    auto iter = guild_list_.find(guild_id);
    if(iter != guild_list_.end())
    {
        guild_list_.erase(iter);

        if(GetScene()->HasObservers(GetObjectId()))
        {
            DeltasMessage message = CreateDeltasMessage(BaseObject::VIEW_3);
            message.data.write<uint16_t>(4);
            message.data.write<uint32_t>(guild_list_.size());
            message.data.write<uint32_t>(guild_list_counter_++);
            message.data.write<uint8_t>(0);
            std::stringstream guild_id_tag;
            guild_id_tag << guild_id << ":" << iter->second;
            message.data.write<std::string>(guild_id_tag.str());
            GetScene()->UpdateObservers(GetObjectId(), message);
            deltas_cache_.push_back(std::make_pair(BaseObject::VIEW_3, std::move(message)));
        }
    }
}

boost::optional<BaselinesMessage> Guild::GetBaseline3()
{
    auto message = CreateBaselinesMessage(BaseObject::VIEW_3, 5);
    message.data.append(BaseObject::GetBaseline3().get().data);
    message.data.write<uint32_t>(guild_list_.size());
    message.data.write<uint32_t>(guild_list_counter_);
    std::for_each(guild_list_.begin(), guild_list_.end(), [=, &message](std::pair<uint32_t, std::string> guild) {
        std::stringstream guild_id_tag;
        guild_id_tag << guild.first << ":" << guild.second;
        // Write each guild as ID:TAG (i.e. "1234:DARK")
        message.data.write<std::string>(guild_id_tag.str());
    });
}

}} // namespaces