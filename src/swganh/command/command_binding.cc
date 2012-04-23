#include "command_binding.h"
#include "combat_binding.h"

BOOST_PYTHON_MODULE(py_command)
{
    exportCommand();
    exportCombatCommand();
}