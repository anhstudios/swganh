// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_
#define SWGANH_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_

#include "swganh/command/v2/command_queue_manager_interface.h"

namespace swganh_core {
namespace command {

    class CommandQueueManager : public swganh::command::v2::CommandQueueManagerInterface
    {};

}}  // namespace swganh_core::command

#endif  // SWGANH_CORE_COMMAND_COMMAND_QUEUE_MANAGER_H_
