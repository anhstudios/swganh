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
}
namespace command {
    class BaseSwgCommand;
}}

namespace pub14_core {
namespace command {

    class CommandQueue : public swganh::command::CommandQueueInterface
    {
    public:
        CommandQueue(swganh::app::SwganhKernel* kernel);
        ~CommandQueue();
        
        virtual void EnqueueCommand(std::unique_ptr<swganh::command::CommandInterface> command);

    private:
        void ProcessCommand(std::unique_ptr<swganh::command::BaseSwgCommand> command);
        
        void Notify();

        template<typename T>
        struct CommandComparator
        {
            bool operator() (const T& t1, const T& t2)
            {
                return (t1->GetPriority() < t2->GetPriority())
                    ? true : false;
            }
        };

        typedef std::priority_queue<
            std::unique_ptr<swganh::command::BaseSwgCommand>, 
            std::vector<std::unique_ptr<swganh::command::BaseSwgCommand>>, 
            CommandComparator<std::unique_ptr<swganh::command::BaseSwgCommand>>
        > ProcessQueue;

        swganh::app::SwganhKernel* kernel_;
        swganh::command::CommandService* command_service_;

        boost::asio::deadline_timer	timer_;
        
        boost::mutex process_mutex_;
        bool processing_;
        
        boost::mutex queue_mutex_;        
        ProcessQueue queue_;
    };

}}  // namespace pub14_core::command

#endif  // PUB14_CORE_COMMAND_COMMAND_QUEUE_H_
