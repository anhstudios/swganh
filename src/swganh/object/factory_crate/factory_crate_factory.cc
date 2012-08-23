// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/object/factory_crate/factory_crate_factory.h"

#include "swganh/object/factory_crate/factory_crate.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object::factory_crate;

uint32_t FactoryCrateFactory::PersistObject(const shared_ptr<Object>& object)
{
	uint32_t counter = 1;
	return counter;
}

void FactoryCrateFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> FactoryCrateFactory::CreateObjectFromStorage(uint64_t object_id)
{
    return make_shared<FactoryCrate>();
}

shared_ptr<Object> FactoryCrateFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	if(db_persisted || db_initialized)
	{
		//TODO: Have to hit the db to make this
		return make_shared<FactoryCrate>();
	}
	else
	{
		return make_shared<FactoryCrate>();
	}
}
