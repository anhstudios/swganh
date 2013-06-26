// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/scripting/python_shared_ptr.h"
#include "swganh_core/spawn/spawn_service_binding.h"

BOOST_PYTHON_MODULE(py_spawn) 
{
    bp::docstring_options local_docstring_options(true, true, false);

    exportSpawnService();
}