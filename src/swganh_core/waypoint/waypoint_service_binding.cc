// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "waypoint_service_binding.h"

BOOST_PYTHON_MODULE(py_waypoint) 
{
    docstring_options local_docstring_options(true, true, false);

    exportWaypointService();
}