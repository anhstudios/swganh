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

#ifndef SWGANH_OBJECT_GUILD_GUILD_H_
#define SWGANH_OBJECT_GUILD_GUILD_H_

#include <mutex>
#include <sstream>

#include "swganh/object/object.h"

#include "swganh/messages/containers/network_list.h"

namespace swganh {
namespace object {
namespace guild {

struct GuildTag
{
    GuildTag(uint32_t id_, std::string abbreviation_)
        : id(id_)
        , abbreviation(abbreviation_)
    {}

    ~GuildTag(void)
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        std::stringstream ss;
        ss << id << ":" << abbreviation;

        message.data.write<std::string>(ss.str());
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        std::stringstream ss;
        ss << id << ":" << abbreviation;

        message.data.write<std::string>(ss.str());
    }

    bool operator==(const GuildTag& other)
    {
        return id == other.id;
    }

    uint32_t id;
    std::string abbreviation;
};

class GuildFactory;
class GuildMessageBuilder;
class Guild : public swganh::object::Object {
public:
    Guild();
    ~Guild();

    void AddGuildTag(uint32_t guild_id, std::string abbreviation);
    void RemoveGuildTag(uint32_t guild_id);
    swganh::messages::containers::NetworkList<GuildTag> GetGuildList();

    virtual uint32_t GetType() const { return type; }
    const static uint32_t type = 0x444C4947;

    boost::optional<swganh::messages::BaselinesMessage> GetBaseline3();
    boost::optional<swganh::messages::BaselinesMessage> GetBaseline6();

private:
    friend class GuildFactory;

    mutable std::mutex guild_mutex_;
    swganh::messages::containers::NetworkList<GuildTag>     guild_list_;
};

}}}  // namespaces swganh::object::guild

#endif // SWGANH_OBJECT_GUILD_GUILD_H_
