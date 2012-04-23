#include "swganh/combat/combat_service_binding.h"

BOOST_PYTHON_MODULE(py_combat) 
{
    docstring_options local_docstring_options(true, true, false);

    exportCombatService();
}