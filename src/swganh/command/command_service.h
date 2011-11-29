
#ifndef SWGANH_COMMAND_COMMAND_SERVICE_H_
#define SWGANH_COMMAND_COMMAND_SERVICE_H_

#include <cstdint>
#include <memory>
#include <string>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>

#include "swganh/base/base_service.h"
#include "swganh/messages/obj_controller_message.h"
#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace swganh {
namespace object {
    class Object;
    class ObjectController;
}}  // namespace swganh::object;

namespace swganh {
namespace command {

    typedef std::function<void (
        uint64_t, // object 
        uint64_t, // target
        std::wstring command_options)
    > CommandHandler;

    typedef tbb::concurrent_queue<
        swganh::messages::controllers::CommandQueueEnqueue
    > CommandQueue;

    class CommandService: public swganh::base::BaseService
    {
    public:
        explicit CommandService(anh::app::KernelInterface* kernel);
        
        anh::service::ServiceDescription GetServiceDescription();

        void AddCommandHandler(uint32_t command_crc, CommandHandler&& handler);

    private:
        
        void HandleCommandQueueEnqueue(
            const std::shared_ptr<swganh::object::ObjectController>& controller, 
            const swganh::messages::ObjControllerMessage& message);

        void HandleCommandQueueRemove(
            const std::shared_ptr<swganh::object::ObjectController>& controller, 
            const swganh::messages::ObjControllerMessage& message);

        void ProcessNextCommand();

        void onStart();

        typedef tbb::concurrent_unordered_map<
            uint32_t, 
            CommandHandler
        > HandlerMap;

        typedef tbb::concurrent_unordered_map<
            uint64_t, 
            CommandQueue
        > CommandQueueMap;
        
        HandlerMap handlers_;
        CommandQueueMap command_queues_;
    };

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_COMMAND_SERVICE_H_
