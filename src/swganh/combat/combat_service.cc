
#include "swganh/combat/combat_service.h"

#include <cctype>

#include "anh/app/kernel_interface.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/crc.h"
#include "anh/database/database_manager_interface.h"
#include "anh/service/service_manager.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/object_controller.h"

#include "swganh/command/command_service.h"
#include "swganh/simulation/simulation_service.h"

using namespace anh::app;
using namespace anh::service;
using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::simulation;
using namespace swganh::combat;
using namespace swganh::command;

CombatService::CombatService(KernelInterface* kernel)
: BaseService(kernel)
{}

ServiceDescription CombatService::GetServiceDescription()
{
    ServiceDescription service_description(
        "CombatService",
        "Combat",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0);

    return service_description;
}

void CombatService::onStart()
{
	simulation_service_ = kernel()->GetServiceManager()
        ->GetService<SimulationService>("SimulationService");

	command_service_ = kernel()->GetServiceManager()
		->GetService<CommandService>("CommandService");
    
	LoadProperties();
}

void CombatService::RegisterCombatHandler(uint32_t command_crc, CombatHandler&& handler)
{
	handlers_[command_crc] = move(handler);
}

void CombatService::LoadProperties()
{    
	if (command_service_ == nullptr)
		return;

    auto command_properties = command_service_->GetCommandProperties();
	for_each(begin(command_properties), end(command_properties), [=] (pair<uint32_t, CommandProperties> command){
		// load up all the combat commands into their own map
		if (command.second.add_to_combat_queue || command.second.command_group == 1 || command.second.command_group == 2)
		{
			combat_properties_map_.insert(command);
		}
		DLOG(WARNING) << "Loaded (" << combat_properties_map_.size() << ") Combat Commands";
	});
}

