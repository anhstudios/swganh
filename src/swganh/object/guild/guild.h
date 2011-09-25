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

#include <map>

#include "swganh/object/object.h"

namespace swganh {
namespace object {
namespace guild {

class Guild : public swganh::object::Object {
public:
    Guild();
    Guild(std::map<uint32_t, std::string> guild_list);
    ~Guild();

    void AddGuild(uint32_t, std::string);
    void RemoveGuild(uint32_t);

    std::map<uint32_t, std::string> GetGuilds() { return guild_list_; }
    uint32_t GetGuildsListCounter() { return guild_list_counter_; }
    void IncrementGuildsListCounter() { guild_list_counter_++; }

    virtual uint32_t GetType() const { return type; }
    const static uint32_t type = 0x444C4947;

private:
    std::map<uint32_t, std::string>     guild_list_;
    uint32_t    guild_list_counter_;
};

}}}  // namespaces swganh::object::guild

#endif // SWGANH_OBJECT_GUILD_GUILD_H_
