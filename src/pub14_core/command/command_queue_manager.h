// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_
#define PUB14_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_

#include <cstdint>
#include <map>
#include <vector>

#include "swganh/command/v2/command_factory_interface.h"
#include "swganh/command/v2/command_queue_interface.h"
#include "swganh/command/v2/command_queue_manager_interface.h"
#include "swganh/command/v2/command_properties_loader_interface.h"
#include "swganh/command/v2/command_service_interface.h"

#include "swganh/simulation/simulation_service.h"

namespace pub14_core {
namespace command {

    class CommandQueueManager : public swganh::command::v2::CommandQueueManagerInterface
    {
    public:
        explicit CommandQueueManager(
            swganh::command::v2::CommandServiceInterface* command_service,
            swganh::simulation::SimulationService* simulation_service,
            swganh::command::v2::CommandPropertiesLoaderInterface* command_properties_loader,
            swganh::command::v2::CommandFactoryInterface* command_factory);

        void EnqueueCommand(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            swganh::messages::controllers::CommandQueueEnqueue message);
        
        swganh::command::v2::CommandQueueInterface* FindCommandQueue(uint64_t object_id);

        boost::optional<const swganh::command::v2::CommandProperties&> FindCommandProperties(uint32_t command_crc);
        
    private:
        CommandQueueManager();

        bool ValidateCommandRequest(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& message);
        
        swganh::command::v2::CommandServiceInterface* command_service_;
        swganh::simulation::SimulationService* simulation_service_;
        
        swganh::command::v2::CommandPropertiesMap command_properties_map_;

        swganh::command::v2::CommandFactoryInterface* command_factory_;

        typedef std::map<
            uint64_t, 
            std::unique_ptr<swganh::command::v2::CommandQueueInterface>
        > CommandQueueMap;

        CommandQueueMap command_queue_map_;
    };

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_
