// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef PUB14_CORE_COMMAND_COMMAND_QUEUE_H_
#define PUB14_CORE_COMMAND_COMMAND_QUEUE_H_

#include <functional>
#include <memory>
#include <queue>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/thread/mutex.hpp>

#include "swganh/command/command_queue_interface.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}}  // namespace swganh::app

namespace pub14_core {
namespace command {

    class CommandQueue : public swganh::command::CommandQueueInterface
    {
    public:
        CommandQueue(swganh::app::SwganhKernel* kernel);
        ~CommandQueue();
        
        void EnqueueCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command,
            const swganh::command::CommandProperties& properties,
            const swganh::command::CommandHandler& handler);

    private:
        void ProcessCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command,
            const swganh::command::CommandProperties& properties,
            const swganh::command::CommandHandler& handler);
        
        void Notify();
                   
        typedef std::function<void ()> Task;
        struct TaskInfo
        {
            TaskInfo(boost::posix_time::time_duration delay, Task&& task)
                : delay(delay), task(std::move(task))
            {}
                
            boost::posix_time::time_duration delay;
            Task task;
        };

        swganh::app::SwganhKernel* kernel_;
        swganh::command::CommandService* command_service_;

        boost::asio::deadline_timer	timer_;
        
        boost::mutex process_mutex_;
        bool processing_;
        
        boost::mutex queue_mutex_;	
        std::queue<std::shared_ptr<TaskInfo>> queue_;
    };

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_QUEUE_H_
