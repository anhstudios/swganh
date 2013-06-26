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

#include "swganh_core/command/command_interface.h"
#include "swganh_core/command/command_queue_interface.h"

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

	/**
	* Represents the queue of commands for a particular client
	* This class wraps a simple queue and adds proper client updating functionality
	*/
    class CommandQueue : public swganh::command::CommandQueueInterface
    {
    public:
		/**
		* Create a new instance
		*/
        CommandQueue(swganh::app::SwganhKernel* kernel);
        
		/**
		* Custom destructor
		*/
		~CommandQueue();
        
		/**
		* Adds a command to this queue
		* @param command the command to add
		*/
        virtual void EnqueueCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);
        
		/**
		* Sets the default command for this queue.
		* @param command the default
		*/
		virtual void SetDefaultCommand(const std::shared_ptr<swganh::command::CommandInterface>& command);
        
		/**
		* Clears the default command for this queue
		*/
		virtual void ClearDefaultCommand();

		/*
		* @return true if this queue has a default command
		*/
        virtual bool HasDefaultCommand();

    private:
        void ProcessCommand(const std::shared_ptr<swganh::command::BaseSwgCommand>& command);
        
        void Notify();
        
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
