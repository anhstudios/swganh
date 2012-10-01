// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "harvester_installation_factory.h"

#include "harvester_installation.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::object;

HarvesterInstallationFactory::HarvesterInstallationFactory(swganh::database::DatabaseManagerInterface* db_manager, swganh::EventDispatcher* event_dispatcher)
	: InstallationFactory(db_manager, event_dispatcher)
{
}
