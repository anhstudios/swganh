// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "factory_crate_factory.h"

#include "factory_crate.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;

FactoryCrateFactory::FactoryCrateFactory(swganh::database::DatabaseManagerInterface* db_manager, swganh::EventDispatcher* event_dispatcher)
	: TangibleFactory(db_manager, event_dispatcher)
{
}
uint32_t FactoryCrateFactory::PersistObject(const shared_ptr<Object>& object)
{
	return 0;
}

void FactoryCrateFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> FactoryCrateFactory::CreateObjectFromStorage(uint64_t object_id)
{
	//@TODO: Load me from storage
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
