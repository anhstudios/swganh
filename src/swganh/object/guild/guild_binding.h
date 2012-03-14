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

#ifndef SWGANH_OBJECT_GUILD_GUILD_BINDING_H_
#define SWGANH_OBJECT_GUILD_GUILD_BINDING_H_

#include "swganh/object/guild/guild.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object::guild;

struct GuildWrapper : Guild,  wrapper<Guild>
{
};

void exportGuild()
{
	class_<GuildWrapper, bases<swganh::object::Object>, boost::noncopyable>("Guild")
		.def("add_guild_tag", &GuildWrapper::AddGuildTag, "adds guild tag to the specified guild_id")
		.def("remove_guild_tag", &GuildWrapper::RemoveGuildTag, "remove guild tag")
		;
}

#endif // SWGANH_OBJECT_GUILD_GUILD_BINDING_H_
