// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "attributes_get_batch_command.h"

#include <boost/algorithm/string.hpp>

#include "swganh/logger.h"
#include "swganh/service/service_manager.h"
#include "swganh/app/swganh_kernel.h"

#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"

#include "swganh/simulation/simulation_service_interface.h"

#include "attributes_service.h"

using swganh::app::SwganhKernel;
using swganh::attributes::AttributesService;
using swganh::attributes::GetAttributesBatchCommand;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandCallback;
using swganh::command::CommandProperties;
using swganh::simulation::SimulationServiceInterface;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;


GetAttributesBatchCommand::GetAttributesBatchCommand(
    SwganhKernel* kernel,
    const CommandProperties& properties)
    : BaseSwgCommand(kernel, properties)
{
    attributes_service_ = kernel->GetServiceManager()->GetService<AttributesService>("AttributesService");
	simulation_service_ = kernel->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
}

GetAttributesBatchCommand::~GetAttributesBatchCommand()
{}

boost::optional<std::shared_ptr<CommandCallback>> GetAttributesBatchCommand::Run()
{
	std::wstring command_str = GetCommandString();
	std::vector<std::string> objects;
	boost::trim(command_str);
	boost::split(objects, command_str, boost::is_any_of(" "));
	auto actor = GetActor();
	for (auto& object : objects)
	{
		auto found = simulation_service_->GetObjectById(boost::lexical_cast<uint64_t>(object));
		if (found)
		{
			attributes_service_->HandleGetAttributesBatch(found, actor);
		}
	}

    return boost::optional<std::shared_ptr<CommandCallback>>();
}
