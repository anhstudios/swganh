// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <boost/python/module.hpp>

#include "controllers/controllers_binding.h"
#include "update_containment_message_binding.h"
#include "update_pvp_status_message_binding.h"
#include "update_transform_message_binding.h"
#include "update_transform_with_parent_message_binding.h"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(py_messages)
{
    bp::scope current;

    exportUpdateContainmentMessage();
    exportUpdatePvpStatusMessage();
    exportUpdateTransformMessage();
    exportUpdateTransformWithParentMessage();
    
    bp::object submodule(bp::borrowed(PyImport_AddModule("swgpy.messages.controllers")));
    current.attr("controllers") = submodule;

    {
        bp::scope controllers_submodule = submodule;
        exportCommandQueueEnqueueController();
    }
}
