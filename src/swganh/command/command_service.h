// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_SERVICE_H_
#define SWGANH_COMMAND_COMMAND_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/optional.hpp>

#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/command/command_properties.h"

#include "command_factory_interface.h"
#include "command_validator_interface.h"

namespace swganh {
namespace object {
    class Object;
    class ObjectController;
namespace tangible {
    class Tangible;
}
namespace creature {
	class Creature;
}} // namespace object::creature

namespace simulation {
    class SimulationService;
}}  // namespace swganh::simulation;


namespace swganh {
namespace command {

    class BaseSwgCommand;
    class CommandInterface;
    class CommandQueueInterface;
    class CommandQueueManagerInterface;
    class CommandPropertiesManagerInterface;
    struct CommandProperties;

    class CommandService: public anh::service::ServiceInterface
    {
    public:
        explicit CommandService(swganh::app::SwganhKernel* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();

        void AddCommandEnqueueFilter(CommandFilter&& filter);
        
        void AddCommandProcessFilter(CommandFilter&& filter);
        
        void AddCommandCreator(anh::HashString command, CommandCreator&& creator);

        void RemoveCommandCreator(anh::HashString command);

        void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            uint32_t action_counter,
            float default_time_sec,
            uint32_t error,
            uint32_t action);
        
        std::tuple<bool, uint32_t, uint32_t> ValidateForEnqueue(CommandInterface* command);
        
        std::tuple<bool, uint32_t, uint32_t> ValidateForProcessing(CommandInterface* command);

        boost::optional<const CommandProperties&> FindPropertiesForCommand(anh::HashString command);

        void Start();

    private:
        void SubscribeObjectReadyEvent(anh::EventDispatcher* dispatcher);

        void SubscribeObjectRemovedEvent(anh::EventDispatcher* dispatcher);

        void HandleCommandQueueEnqueue(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            swganh::messages::controllers::CommandQueueEnqueue message);
                
        swganh::app::SwganhKernel* kernel_;
        std::shared_ptr<CommandFactoryInterface> command_factory_impl_;
        std::shared_ptr<CommandPropertiesManagerInterface> command_properties_manager_impl_;
        std::shared_ptr<CommandQueueManagerInterface> command_queue_manager_impl_;
        std::shared_ptr<CommandValidatorInterface> command_validator_impl_;
        swganh::simulation::SimulationService* simulation_service_;
        std::string script_prefix_;
    };

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_COMMAND_SERVICE_H_
