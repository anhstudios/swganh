// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_queue.h"

#include "anh/logger.h"

#include "anh/service/service_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/command/command_service.h"

#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"
#include "swganh/object/object_controller.h"

using pub14_core::command::CommandQueue;
using swganh::command::CommandService;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;

CommandQueue::CommandQueue(
    swganh::app::SwganhKernel* kernel)
    : kernel_(kernel)
    , timer_(kernel->GetIoService())
{
    command_service_ = kernel->GetServiceManager()->GetService<CommandService>("CommandService");
}

CommandQueue::~CommandQueue()
{}

void CommandQueue::EnqueueCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command,
            const swganh::command::CommandProperties& properties,
            const swganh::command::CommandHandler& handler)
{
    if (!command_service_->ValidateCommandForEnqueue(actor, target, command, properties))
    {
        return;
    }

    if (properties.add_to_combat_queue)
    {
        queue_.push(std::make_shared<TaskInfo>(
            &properties, 
            bind(&CommandQueue::ProcessCommand, this, actor, target, command, properties, handler)));

        Notify();
    }
    else
    {
        ProcessCommand(actor, target, command, properties, handler);
    }

}

void CommandQueue::ProcessCommand(
            const std::shared_ptr<swganh::object::creature::Creature>& actor,
            const std::shared_ptr<swganh::object::tangible::Tangible>& target,
            const swganh::messages::controllers::CommandQueueEnqueue& command,
            const swganh::command::CommandProperties& properties,
            const swganh::command::CommandHandler& handler)
{
    try {
        if (command_service_->ValidateCommandForProcessing(actor, target, command, properties))
        {
		    handler(kernel_, actor, target, command);
            
            command_service_->SendCommandQueueRemove(actor, command.action_counter, properties.default_time, 0, 0);
        }
    } catch(const std::exception& e) {
        LOG(warning) << "Error Processing Command: " <<  properties.command_name.ident_string() << "\n" << e.what();
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
            auto& task_info = queue_.front();
            queue_.pop();

            task_info->task();
            timer_.expires_from_now(boost::posix_time::milliseconds(static_cast<uint64_t>(task_info->properties->default_time * 1000)));
            timer_.async_wait([this, task_info] (const boost::system::error_code& ec) 
            {
                {
                    boost::lock_guard<boost::mutex> lg(process_mutex_);
                    processing_ = false;
                }

                this->Notify();
            });
        }
    }		
}
