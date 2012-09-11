// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <functional>
#include <memory>
#include <queue>

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/thread/mutex.hpp>

#include "swganh/active_object.h"

#include "swganh/command/command_interface.h"
#include "swganh/command/command_queue_interface.h"

namespace swganh {
namespace app {
    class SwganhKernel;
}
namespace command {
    class BaseSwgCommand;
    class CommandServiceInterface;
}}

namespace swganh {
namespace command {

    class CommandQueue : public swganh::command::CommandQueueInterface
    {
    public:
        CommandQueue(swganh::app::SwganhKernel* kernel);
        ~CommandQueue();
        
        virtual void EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);
        virtual void SetDefaultCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);
        virtual void ClearDefaultCommand();
        virtual bool HasDefaultCommand();

    private:
        void ProcessCommand(const std::shared_ptr<swganh::command::BaseSwgCommand>& command);
        
        void Notify();
        
        void HandleCallback(std::shared_ptr<swganh::command::CommandCallback> callback);

        std::shared_ptr<swganh::command::BaseSwgCommand> GetNextCommand();

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
            std::shared_ptr<swganh::command::BaseSwgCommand>, 
            std::vector<std::shared_ptr<swganh::command::BaseSwgCommand>>, 
            CommandComparator<std::shared_ptr<swganh::command::BaseSwgCommand>>
        > ProcessQueue;

        swganh::app::SwganhKernel* kernel_;
        swganh::command::CommandServiceInterface* command_service_;

        boost::asio::deadline_timer	timer_;
        
        boost::mutex process_mutex_;
        bool processing_;
        
        boost::mutex queue_mutex_;        
        ProcessQueue queue_;

        std::shared_ptr<swganh::command::BaseSwgCommand> default_command_;

        swganh::ActiveObject active_;
    };

}}  // namespace swganh::command
