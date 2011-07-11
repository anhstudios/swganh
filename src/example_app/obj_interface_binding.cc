
#include "example_app/obj_interface.h"

#include <boost/python.hpp>

using namespace boost::python;

struct ObjInterfaceWrapper : ObjInterface, wrapper<ObjInterface> {
    std::string DoWork() {
        object o = this->get_override("DoWork")();
        std::string tmp = extract<std::string>(o);

        return tmp;
    }
};

BOOST_PYTHON_MODULE(example_app_binding) {
    class_<ObjInterfaceWrapper, boost::noncopyable>("ObjInterface")
        .def("DoWork", pure_virtual(&ObjInterface::DoWork))
    ;
}
