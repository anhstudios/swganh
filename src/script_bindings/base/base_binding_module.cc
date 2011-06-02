#include "glm_bindings.h"
#include <api/components/transform_component_interface.h>
#include <mod_anh_transform/transform_component.h>
#include <script_bindings/components/transform_component_binding.h>
#include <script_bindings/components/base_component_binding.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace transform;
using namespace anh_python::components;

namespace anh_python { namespace scripting {
    BOOST_PYTHON_MODULE(anh_python) {
        define_class_glm_vec3();
        define_class_glm_quat();
        define_class_transform_component();
//        define_class_transform_message();
    }


} // scripting
} // anh_python