// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "python_script_creator.h"

#include <boost/python.hpp>

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
