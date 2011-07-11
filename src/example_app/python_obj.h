
#ifndef EXAMPLE_APP_PYTHON_OBJ_H_
#define EXAMPLE_APP_PYTHON_OBJ_H_

#include "example_app/obj_interface.h"

#include <boost/python/object.hpp>

class PythonObj : public ObjInterface {
public:
    PythonObj(std::string module_name, std::string class_name);

    std::string DoWork();

private:
    boost::python::object py_object_;
    ObjInterface* obj_interface_;
};

#endif  // EXAMPLE_APP_PYTHON_OBJ_H_
