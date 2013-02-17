
#include "python_attributes_creator.h"

#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/attributes/attribute_template_interface.h"
#include "swganh/scripting/utilities.h"

namespace bp = boost::python;

using swganh::scripting::ScopedGilLock;
using namespace swganh::attributes;

PythonAttributesCreator::PythonAttributesCreator(std::string module_name, std::string class_name)
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

std::shared_ptr<AttributeTemplateInterface> PythonAttributesCreator::operator() (swganh::app::SwganhKernel* kernel)
{
    std::shared_ptr<AttributeTemplateInterface> attribute_template = nullptr;
    
    ScopedGilLock lock;

    try 
    {

#ifdef _DEBUG
        module_ = bp::object(bp::handle<>(PyImport_ReloadModule(module_.ptr())));
#endif
        
        auto new_instance = module_.attr(class_name_.c_str())(bp::ptr(kernel));

        if (!new_instance.is_none())
        {
            AttributeTemplateInterface* obj_pointer = bp::extract<AttributeTemplateInterface*>(new_instance);
            attribute_template.reset(obj_pointer, [new_instance] (AttributeTemplateInterface*) {});
        }
    }
    catch(bp::error_already_set& /*e*/)
    {
        PyErr_Print();
    }

    return attribute_template;
}
