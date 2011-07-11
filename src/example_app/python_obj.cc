
#include "example_app/python_obj.h"

#include <boost/python.hpp>

namespace py = boost::python;

PythonObj::PythonObj(std::string module_name, std::string class_name) 
    : obj_interface_(nullptr)
{
    py::object mm = py::import("__main__");
    py::object mn = mm.attr("__dict__");

    py::exec(std::string("import " + module_name).c_str(), mn);

    py_object_ = py::eval(std::string(module_name + "." + class_name + "()").c_str(), mn);

    //py::object py_module = py::import(module_name.c_str());
    //py::object py_class = py_module.attr(class_name.c_str());
    //
    //py_object_ = py_class();
    obj_interface_ = py::extract<ObjInterface*>(py_object_);
}

std::string PythonObj::DoWork() {
    if (!obj_interface_) {
        return "No luck yet";
    }

    return obj_interface_->DoWork();
}
