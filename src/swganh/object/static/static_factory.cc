// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/static/static_factory.h"

#include "swganh/object/static/static.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::static_object;

uint32_t StaticFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;

	return counter;
}

void StaticFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> StaticFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<Static>();
}

shared_ptr<Object> StaticFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	//@TODO: Create me with help from db
    return make_shared<Static>();
}
