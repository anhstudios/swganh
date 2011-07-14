
#ifndef EXAMPLE_APP_PYTHON_OBJ_H_
#define EXAMPLE_APP_PYTHON_OBJ_H_

#include "example_app/obj_interface.h"

#include <boost/python.hpp>

class PythonObj : public ObjInterface {
public:
    PythonObj(std::string module_name, std::string class_name) {        
        boost::python::object mm = boost::python::import("__main__");
        boost::python::object mn = mm.attr("__dict__");

        boost::python::exec(std::string("import " + module_name).c_str(), mn);

        py_object_ = boost::python::eval(std::string(module_name + "." + class_name + "()").c_str(), mn);

        obj_interface_ = boost::python::extract<ObjInterface*>(py_object_);
    }

    std::string DoWork() {
        return obj_interface_->DoWork();
    }

private:
    boost::python::object py_object_;
    ObjInterface* obj_interface_;
};

#endif  // EXAMPLE_APP_PYTHON_OBJ_H_
