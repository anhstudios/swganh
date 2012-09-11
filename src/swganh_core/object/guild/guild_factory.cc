// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "guild_factory.h"

#include "guild.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

uint32_t GuildFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;

	return counter;
}

void GuildFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> GuildFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Guild>();
}

shared_ptr<Object> GuildFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	if(db_persisted || db_initialized)
	{
		//TODO: Have to hit the db to make this
		return make_shared<Guild>();
	}
	else
	{
		return make_shared<Guild>();
	}
}
