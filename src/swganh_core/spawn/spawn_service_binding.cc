// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "spawn_service_binding.h"

BOOST_PYTHON_MODULE(py_spawn) 
{
    docstring_options local_docstring_options(true, true, false);

    exportSpawnService();
}