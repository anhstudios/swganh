
#include "python_radial_creator.h"

#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/sui/radial_interface.h"
#include "swganh/scripting/utilities.h"

namespace bp = boost::python;

using swganh::scripting::ScopedGilLock;
using namespace swganh::sui;

PythonRadialCreator::PythonRadialCreator(std::string module_name, std::string class_name)
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

std::shared_ptr<RadialInterface> PythonRadialCreator::operator() (swganh::app::SwganhKernel* kernel)
{
    std::shared_ptr<RadialInterface> radial = nullptr;
    
    ScopedGilLock lock;

    try 
    {

#ifdef _DEBUG
        module_ = bp::object(bp::handle<>(PyImport_ReloadModule(module_.ptr())));
#endif
        
        auto new_instance = module_.attr(class_name_.c_str())(bp::ptr(kernel));

        if (!new_instance.is_none())
        {
            RadialInterface* obj_pointer = bp::extract<RadialInterface*>(new_instance);
            radial.reset(obj_pointer, [new_instance] (RadialInterface*) {});
        }
    }
    catch(bp::error_already_set& /*e*/)
    {
        PyErr_Print();
    }

    return radial;
}
