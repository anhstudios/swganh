// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_combat_command.h"

#include "anh/service/service_manager.h"

#include "swganh/combat/combat_service.h"

using swganh::combat::CombatService;
using swganh::command::BaseCombatCommand;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandCallback;
using swganh::command::CommandProperties;
using swganh::object::ObjectController;

BaseCombatCommand::BaseCombatCommand(
    swganh::app::SwganhKernel* kernel,
    const CommandProperties& properties,
    const std::shared_ptr<ObjectController>& controller,
    const swganh::messages::controllers::CommandQueueEnqueue& command_request)
    : BaseSwgCommand(kernel, properties, controller, command_request)
{}

BaseCombatCommand::~BaseCombatCommand()
{}

boost::optional<std::shared_ptr<CommandCallback>> BaseCombatCommand::Run()
{
    auto combat_service = GetKernel()->GetServiceManager()->GetService<CombatService>("CombatService");

    combat_service->SendCombatAction(this);

    return boost::optional<std::shared_ptr<CommandCallback>>();
}
