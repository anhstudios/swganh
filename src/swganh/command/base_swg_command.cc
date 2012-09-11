// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_swg_command.h"

#include "swganh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/object.h"
#include "swganh/simulation/simulation_service_interface.h"

using swganh::observer::ObserverInterface;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandProperties;
using swganh::object::Object;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::simulation::SimulationServiceInterface;

BaseSwgCommand::BaseSwgCommand(
    swganh::app::SwganhKernel* kernel,
    const CommandProperties& properties)
    : kernel_(kernel)
    , properties_(&properties)
    , actor_(nullptr)
    , target_(nullptr)
{}

BaseSwgCommand::~BaseSwgCommand()
{}

const std::shared_ptr<ObserverInterface> BaseSwgCommand::GetController() const
{
	return controller_;
}

void BaseSwgCommand::SetController(std::shared_ptr<swganh::observer::ObserverInterface> controller)
{
	controller_ = controller;
}

bool BaseSwgCommand::Validate()
{
    return true;
}

swganh::app::SwganhKernel* BaseSwgCommand::GetKernel() const
{
    return kernel_;
}
        
std::string BaseSwgCommand::GetCommandName() const
{
    return properties_->command_name.ident_string();
}

uint32_t BaseSwgCommand::GetActionCounter() const
{
    return command_request_.action_counter;
}

uint32_t BaseSwgCommand::GetPriority() const
{
    return properties_->default_priority;
}

uint32_t BaseSwgCommand::GetCommandGroup() const
{
    return properties_->command_group;
}

uint32_t BaseSwgCommand::GetTargetRequiredType() const
{
    return properties_->target_type;
}

uint64_t BaseSwgCommand::GetAllowedStateBitmask() const
{
    return properties_->allow_in_state;
}

float BaseSwgCommand::GetMaxRangeToTarget() const
{
    return properties_->max_range_to_target;
}

float BaseSwgCommand::GetDefaultTime() const
{
    return properties_->default_time;
}
        
std::string BaseSwgCommand::GetRequiredAbility() const
{
    return properties_->character_ability.ident_string();
}

bool BaseSwgCommand::IsQueuedCommand() const
{
    return properties_->add_to_combat_queue != 0;
}

const std::shared_ptr<Object>& BaseSwgCommand::GetActor() const
{
    if (!actor_)
    {
        auto simulation_service = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
        actor_ = simulation_service->GetObjectById(command_request_.observable_id);
    }

    return actor_;
}

void BaseSwgCommand::SetActor(std::shared_ptr<Object> object)
{
	actor_ = object;
}

const std::shared_ptr<Object>& BaseSwgCommand::GetTarget() const
{
    if (!target_)
    {
        auto simulation_service = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
        target_ = simulation_service->GetObjectById(command_request_.target_id);
    }

    return target_;
}

void BaseSwgCommand::SetTarget(std::shared_ptr<Object> object)
{
	target_ = object;
}

void BaseSwgCommand::SetCommandProperties(const CommandProperties& properties)
{
    properties_ = &properties;
}
        
const std::wstring& BaseSwgCommand::GetCommandString() const
{
    return command_request_.command_options;
}
        
const CommandQueueEnqueue& BaseSwgCommand::GetCommandRequest() const
{
    return command_request_;
}

void BaseSwgCommand::SetCommandRequest(swganh::messages::controllers::CommandQueueEnqueue command_request)
{
    command_request_ = command_request;
}
