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