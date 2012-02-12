
#ifndef SWGANH_COMMAND_COMMAND_SERVICE_H_
#define SWGANH_COMMAND_COMMAND_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <tuple>

#include <boost/asio/deadline_timer.hpp>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>

#include "swganh/base/base_service.h"
#include "swganh/messages/obj_controller_message.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"
#include "swganh/command/command_properties.h"

namespace swganh {
namespace object {
    class Object;
    class ObjectController;
namespace creature {
	class Creature;
}}}  // namespace swganh::object::creature;

namespace swganh {
namespace command {

    typedef std::function<void (
		const std::shared_ptr<swganh::object::creature::Creature>&, // creature object
		const std::shared_ptr<swganh::object::Object>&,	// target object
        std::wstring command_options)
    > CommandHandler;

    typedef std::function<std::tuple<bool, uint32_t, uint32_t> (
        const std::shared_ptr<swganh::object::creature::Creature>&, // creature object
		const std::shared_ptr<swganh::object::Object>&, // target object
        const swganh::messages::controllers::CommandQueueEnqueue&,
        const CommandProperties&)	// action
    > CommandFilter;
    
    class CommandService: public swganh::base::BaseService
    {
    public:
        explicit CommandService(anh::app::KernelInterface* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();

        void AddCommandEnqueueFilter(CommandFilter&& filter);
        
        void AddCommandProcessFilter(CommandFilter&& filter);

        void SetCommandHandler(uint32_t command_crc, CommandHandler&& handler);

        void EnqueueCommand(const std::shared_ptr<swganh::object::creature::Creature>& actor,
			const std::shared_ptr<swganh::object::Object>& target,
			swganh::messages::controllers::CommandQueueEnqueue command);



    private:

        void SendCommandQueueRemove(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const swganh::messages::controllers::CommandQueueEnqueue& command,
            float default_time,
            uint32_t error,
            uint32_t action);

        std::tuple<bool, uint32_t, uint32_t> ValidateCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
			const std::shared_ptr<swganh::object::Object>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command, 
            const CommandProperties& command_properties,
            const std::vector<CommandFilter>& filters);
        
        void ProcessCommand(
			const std::shared_ptr<swganh::object::creature::Creature>& actor,
			const std::shared_ptr<swganh::object::Object>& target,
			const swganh::messages::controllers::CommandQueueEnqueue& command);

        void LoadProperties();

        void RegisterCommandScript(const CommandProperties& properties);
        
        void HandleCommandQueueEnqueue(
            const std::shared_ptr<swganh::object::ObjectController>& controller,
            const swganh::messages::ObjControllerMessage& message);

        void HandleCommandQueueRemove(
            const std::shared_ptr<swganh::object::ObjectController>& controller, 
            const swganh::messages::ObjControllerMessage& message);
                
        void ProcessNextCommand(
			const std::shared_ptr<swganh::object::creature::Creature>& actor);

        void onStart();

        typedef tbb::concurrent_unordered_map<
            uint32_t, 
            CommandHandler
        > HandlerMap;
        
        typedef tbb::concurrent_queue<
            swganh::messages::controllers::CommandQueueEnqueue
        > CommandQueue;

        typedef tbb::concurrent_unordered_map<
            uint64_t, 
            CommandQueue
        > CommandQueueMap;

        typedef tbb::concurrent_unordered_map<
            uint64_t,
            std::shared_ptr<boost::asio::deadline_timer>
        > CommandQueueTimerMap;

        typedef std::map<
            uint32_t,
            CommandProperties
        > CommandPropertiesMap;

        typedef std::map<
            uint32_t,
            std::shared_ptr<boost::asio::deadline_timer>
        > CooldownTimerMap;

        typedef std::map<
            uint64_t,
            CooldownTimerMap
        > PlayerCooldownTimerMap;
        
        HandlerMap handlers_;
        CommandQueueMap command_queues_;
        CommandQueueTimerMap command_queue_timers_;
        CommandPropertiesMap command_properties_map_;
        PlayerCooldownTimerMap cooldown_timers_;
        std::vector<CommandFilter> enqueue_filters_;
        std::vector<CommandFilter> process_filters_;
    };

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_COMMAND_SERVICE_H_
