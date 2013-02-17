// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef WIN32
#include <Python.h>
#endif

#include "base_combat_command.h"

#include "swganh_core/combat/combat_data.h"

#include "swganh/service/service_manager.h"

#include "swganh_core/combat/combat_service_interface.h"

using swganh::combat::CombatServiceInterface;
using swganh::combat::CombatData;
using swganh::command::BaseCombatCommand;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandCallback;
using swganh::command::CommandProperties;
using swganh::object::ObjectController;

BaseCombatCommand::BaseCombatCommand(
    swganh::app::SwganhKernel* kernel,
    CommandProperties& properties)
    : BaseSwgCommand(kernel, properties)
	, combat_data(std::make_shared<CombatData>(properties))
{}

BaseCombatCommand::~BaseCombatCommand()
{}

boost::optional<std::shared_ptr<CommandCallback>> BaseCombatCommand::Run()
{
    auto combat_service = GetKernel()->GetServiceManager()->GetService<CombatServiceInterface>("CombatService");

    combat_service->SendCombatAction(this);

    return boost::optional<std::shared_ptr<CommandCallback>>();
}

void BaseCombatCommand::SetCommandProperties(const CommandProperties& properties)
{
	
}