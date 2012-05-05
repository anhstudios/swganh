// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_queue.h"

#include "anh/logger.h"

#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/command/base_swg_command.h"
#include "swganh/command/command_interface.h"
#include "swganh/command/command_service.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/object_controller.h"

using pub14_core::command::CommandQueue;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandInterface;
using swganh::command::CommandService;
using swganh::object::creature::Creature;
using swganh::object::tangible::Tangible;

CommandQueue::CommandQueue(
    swganh::app::SwganhKernel* kernel)
    : kernel_(kernel)
    , timer_(kernel->GetIoService())
{
    command_service_ = kernel->GetServiceManager()->GetService<CommandService>("CommandService");
}

CommandQueue::~CommandQueue()
{}

void CommandQueue::EnqueueCommand(std::unique_ptr<CommandInterface> command)
{    
    if (!command_service_->ValidateCommandForEnqueue(command.get()))
    {
        return;
    }

    std::unique_ptr<BaseSwgCommand> swg_command(static_cast<BaseSwgCommand*>(command.release()));

    if (swg_command->IsQueuedCommand())
    {
        queue_.push(std::move(swg_command));

        Notify();
    }
    else
    {
        ProcessCommand(std::move(swg_command));
    }
}

void CommandQueue::ProcessCommand(std::unique_ptr<swganh::command::BaseSwgCommand> command)
{
    try {
        if (command_service_->ValidateCommandForProcessing(command.get()))
        {
		    command->Run();
            
            command_service_->SendCommandQueueRemove(command->GetActor(), command->GetActionCounter(), command->GetDefaultTime(), 0, 0);
        }
    } catch(const std::exception& e) {
        LOG(warning) << "Error Processing Command: " <<  command->GetCommandName() << "\n" << e.what();
    }
}

void CommandQueue::Notify()
{
    boost::lock_guard<boost::mutex> lg(process_mutex_);
    if (!processing_)
    {
        processing_ = true;

        boost::lock_guard<boost::mutex> lg(queue_mutex_);
        if (!queue_.empty())
        {
            std::unique_ptr<BaseSwgCommand> command(queue_.top().release());
            queue_.pop();
            
            timer_.expires_from_now(boost::posix_time::milliseconds(static_cast<uint64_t>(command->GetDefaultTime() * 1000)));
            timer_.async_wait([this] (const boost::system::error_code& ec) 
            {
                {
                    boost::lock_guard<boost::mutex> lg(process_mutex_);
                    processing_ = false;
                }

                this->Notify();
            });

            
            ProcessCommand(std::move(command));
        }
    }		
}
