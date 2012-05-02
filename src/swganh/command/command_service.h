// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_COMMAND_SERVICE_H_
#define SWGANH_COMMAND_COMMAND_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/asio/deadline_timer.hpp>
#include <boost/thread/mutex.hpp>

#ifdef WIN32
#include <concurrent_unordered_map.h>
#else
#include <tbb/concurrent_unordered_map.h>

namespace Concurrency {
    using ::tbb::concurrent_unordered_map;
}

#endif

#include "anh/delayed_task_processor.h"
#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/messages/obj_controller_message.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/messages/controllers/command_queue_remove.h"
#include "swganh/command/command_properties.h"


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

    class CommandPropertiesLoaderInterface;

    typedef std::function<void (
        swganh::app::SwganhKernel*,
		const std::shared_ptr<swganh::object::creature::Creature>&, // creature object
		const std::shared_ptr<swganh::object::tangible::Tangible>&,	// target object
        const swganh::messages::controllers::CommandQueueEnqueue&)
    > CommandHandler;

    typedef std::function<std::tuple<bool, uint32_t, uint32_t> (
        const std::shared_ptr<swganh::object::creature::Creature>&, // creature object
		const std::shared_ptr<swganh::object::tangible::Tangible>&, // target object
        const swganh::messages::controllers::CommandQueueEnqueue&,
        const CommandProperties&)	// action
    > CommandFilter;
    
    class CommandService: public anh::service::ServiceInterface
    {
    public:
        explicit CommandService(swganh::app::SwganhKernel* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();

        void AddCommandEnqueueFilter(CommandFilter&& filter);
        
        void AddCommandProcessFilter(CommandFilter&& filter);

        void SetCommandHandler(uint32_t command_crc, CommandHandler&& handler);

        void EnqueueCommand(const std::shared_ptr<swganh::object::creature::Creature>& actor,
			const std::shared_ptr<swganh::object::tangible::Tangible> & target,
			swganh::messages::controllers::CommandQueueEnqueue command);

		CommandPropertiesMap GetCommandProperties() { return command_properties_map_; }
        
        void Start();

    private:
        bool ValidateCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
			const std::shared_ptr<swganh::object::tangible::Tangible> & target,
            const swganh::messages::controllers::CommandQueueEnqueue& command, 
            const CommandProperties& command_properties,
            const std::vector<CommandFilter>& filters);
        
        void ProcessCommand(
			const std::shared_ptr<swganh::object::creature::Creature>& actor,
			const std::shared_ptr<swganh::object::tangible::Tangible> & target,
			const swganh::messages::controllers::CommandQueueEnqueue& command,
            const CommandProperties& properties,
            const CommandHandler& handler);
        
        void RegisterCommandScripts();
        
        void HandleCommandQueueEnqueue(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            swganh::messages::controllers::CommandQueueEnqueue message);

        void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            uint32_t action_counter,
            float default_time_sec,
            uint32_t error,
            uint32_t action);

        typedef std::map<
            uint64_t,
            std::unique_ptr<anh::SimpleDelayedTaskProcessor>
        > CommandProcessorMap;

        typedef Concurrency::concurrent_unordered_map<
            uint32_t, 
            CommandHandler
        > HandlerMap;        
        
        swganh::app::SwganhKernel* kernel_;
        std::unique_ptr<anh::SimpleDelayedTaskProcessor> delayed_task_;
        std::shared_ptr<CommandPropertiesLoaderInterface> command_properties_loader_impl_;
        swganh::simulation::SimulationService* simulation_service_;
        boost::mutex processor_map_mutex_;
        CommandProcessorMap processor_map_;
        HandlerMap handlers_;
        CommandPropertiesMap command_properties_map_;
        std::vector<CommandFilter> enqueue_filters_;
        std::vector<CommandFilter> process_filters_;
        std::string script_prefix_;
    };

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_COMMAND_SERVICE_H_
