#include "glm_bindings.h"
#include <zone/components/transform_component.h>
#include "../components/transform_component_binding.h"
#include "../components/base_component_binding.h"
#include <boost/python.hpp>

using namespace boost::python;
using namespace zone::components;

namespace anh_python { namespace scripting {
    class BaseComponentWrapped : public anh_python::components::BaseComponentBinding<BaseComponent>
    {
    public:
        BaseComponentWrapped() : BaseComponentBinding(0) {}
    };
    BOOST_PYTHON_MODULE(anh_python) {
        define_class_glm_vec3();
        define_class_glm_quat();
        anh_python::components::define_class_transform_component();
    }


} // scripting
} // anh_python