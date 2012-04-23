// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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
