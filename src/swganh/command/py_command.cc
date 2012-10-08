// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <boost/python/module.hpp>

#include "base_combat_command_binding.h"
#include "command_binding.h"
#include "command_properties_binding.h"
#include "command_service_binding.h"

BOOST_PYTHON_MODULE(py_command)
{
    swganh::command::ExportCommand();
    swganh::command::ExportBaseCombatCommand();
    swganh::command::ExportCommandProperties();
    swganh::command::ExportCommandService();
}
