// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include "swganh_core/object/guild/guild.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::object;

struct GuildWrapper : Guild,  wrapper<Guild>
{
};

void exportGuild()
{
	class_<GuildWrapper, bases<swganh::object::Object>, boost::noncopyable>("Guild")
		.def("addGuildTag", &GuildWrapper::AddGuildTag, "adds guild tag to the specified guild_id")
		.def("removeGuildTag", &GuildWrapper::RemoveGuildTag, "remove guild tag")
		;	
}
