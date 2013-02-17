
#include "python_buff_creator.h"
#include "buff_interface.h"

#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/scripting/utilities.h"

namespace bp = boost::python;

using swganh::scripting::ScopedGilLock;
using namespace swganh::combat;

PythonBuffCreator::PythonBuffCreator(std::string module_name, std::string class_name)
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

std::shared_ptr<BuffInterface> PythonBuffCreator::operator() ()
{
    std::shared_ptr<BuffInterface> buff = nullptr;
    
    ScopedGilLock lock;

    try 
    {

#ifdef _DEBUG
        module_ = bp::object(bp::handle<>(PyImport_ReloadModule(module_.ptr())));
#endif
        
        auto new_instance = module_.attr(class_name_.c_str())();
        if (!new_instance.is_none())
        {
            BuffInterface* obj_pointer = bp::extract<BuffInterface*>(new_instance);
            buff.reset(obj_pointer, [new_instance] (BuffInterface*) {});
        }
    }
    catch(bp::error_already_set& /*e*/)
    {
        PyErr_Print();
    }

    return buff;
}
