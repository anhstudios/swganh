// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <boost/python.hpp>

#include "glm_binding.h"

using namespace boost::python;
using namespace swganh::utilities;

BOOST_PYTHON_MODULE(py_utility) 
{
    docstring_options local_docstring_options(true, true, false);

    define_class_glm_vec3();
    define_class_glm_quat();
    
}