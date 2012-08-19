
#include "python_script_creator.h"

#include <boost/python.hpp>

#include "swganh/scripting/utilities.h"

namespace bp = boost::python;
using swganh::scripting::ScopedGilLock;
using namespace swganh::scripting;

PythonScriptCreator::PythonScriptCreator(std::string module_name, std::string class_name)
    : module_name_(module_name)
    , class_name_(class_name)
{
    ScopedGilLock lock;

    try 
    {
        module_ = bp::import(module_name_.c_str());
    }
    catch(bp::error_already_set& /*e*/)
    {
        PyErr_Print();
    }
}
