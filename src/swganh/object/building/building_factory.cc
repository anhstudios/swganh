// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/building/building_factory.h"

#include "swganh/object/building/building.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::building;

uint32_t BuildingFactory::PersistObject(const shared_ptr<Object>& object)
{
	return 0;
}

void BuildingFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> BuildingFactory::CreateObjectFromStorage(uint64_t object_id)
{
	//@TODO: Load me from storage
    return make_shared<Building>();
}

shared_ptr<Object> BuildingFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	if(db_persisted || db_initialized)
	{
		//TODO: Have to hit the db to make this
		return make_shared<Building>();
	}
	else
	{
		return make_shared<Building>();
	}
}
