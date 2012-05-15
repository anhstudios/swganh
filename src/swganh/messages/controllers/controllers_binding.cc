// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "controllers_binding.h"

#include <boost/python.hpp>

#include "swganh/messages/controllers/command_queue_enqueue.h"

namespace bp = boost::python;
using swganh::messages::controllers::CommandQueueEnqueue;

void exportCommandQueueEnqueueController()
{
    bp::class_<CommandQueueEnqueue>("CommandQueueEnqueue");
}