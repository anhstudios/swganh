// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_queue_manager.h"

#include "swganh/simulation/simulation_service.h"

using pub14_core::command::CommandQueueManager;
using swganh::command::v2::CommandFactoryInterface;
using swganh::command::v2::CommandInterface;
using swganh::command::v2::CommandProperties;
using swganh::command::v2::CommandPropertiesLoaderInterface;
using swganh::command::v2::CommandQueueInterface;
using swganh::command::v2::CommandServiceInterface;
using swganh::messages::controllers::CommandQueueEnqueue;
using swganh::object::ObjectController;
using swganh::object::creature::Creature;
using swganh::object::tangible::Tangible;
using swganh::simulation::SimulationService;

CommandQueueManager::CommandQueueManager(
    CommandServiceInterface* command_service,
    SimulationService* simulation_service,
    CommandPropertiesLoaderInterface* command_properties_loader,
    CommandFactoryInterface* command_factory)
    : command_service_(command_service)
    , simulation_service_(simulation_service)
    , command_properties_map_(command_properties_loader->LoadCommandPropertiesMap())
    , command_factory_(command_factory)
{}

void CommandQueueManager::EnqueueCommand(
    const std::shared_ptr<ObjectController>& controller,
    CommandQueueEnqueue message)
{
    auto actor = simulation_service_->GetObjectById<Creature>(message.observable_id);
	auto target = simulation_service_->GetObjectById<Tangible>(message.target_id);

    if (!ValidateCommandRequest(actor, target, message))
    {
        return;
    }
    
    FindCommandQueue(actor->GetObjectId())->EnqueueCommand(
        command_factory_->CreateCommand(actor, target, message));
}

CommandQueueInterface* CommandQueueManager::FindCommandQueue(uint64_t object_id)
{
    CommandQueueInterface* command_queue = nullptr;

    auto find_iter = command_queue_map_.find(object_id);
    if (find_iter != command_queue_map_.end())
    {
        command_queue = find_iter->second.get();
    }

    return command_queue;
}

boost::optional<const CommandProperties&> CommandQueueManager::FindCommandProperties(uint32_t command_crc)
{
    boost::optional<const CommandProperties&> command_properties;

    auto find_iter = command_properties_map_.find(command_crc);
    if (find_iter != command_properties_map_.end())
    {
        command_properties = find_iter->second;
    }

    return command_properties;
}
        
bool ValidateCommandRequest(
    const std::shared_ptr<swganh::object::creature::Creature>& actor,
    const std::shared_ptr<swganh::object::tangible::Tangible>& target,
    const swganh::messages::controllers::CommandQueueEnqueue& message)
{

}
