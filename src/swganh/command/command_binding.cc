// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_binding.h"

#include "anh/python_shared_ptr.h"
#include <boost/python.hpp>

#include "swganh/scripting/utilities.h"

#include "base_swg_command.h"
#include "command_interface.h"

namespace bp = boost::python;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::object::ObjectController;
using swganh::scripting::ScopedGilLock;

struct CommandWrapper : CommandInterface, bp::wrapper<CommandInterface>
{    
    const std::shared_ptr<swganh::object::ObjectController>& GetController() const
    {         
        ScopedGilLock lock;
        return this->get_override("GetController")().as<const std::shared_ptr<swganh::object::ObjectController>&>();
    }

    void Setup()
    {
        ScopedGilLock lock;
        this->get_override("Setup")();
    }

    bool Validate()
    {
        ScopedGilLock lock;
        return this->get_override("Validate")();
    }

    void Run()
    {
        ScopedGilLock lock;
        this->get_override("Run")();
    }
};

struct BaseSwgCommandWrapper : BaseSwgCommand, bp::wrapper<BaseSwgCommand>
{
    BaseSwgCommandWrapper(
        PyObject* obj,
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties,
        const std::shared_ptr<ObjectController>& controller,
        const swganh::messages::controllers::CommandQueueEnqueue& command_request)
        : BaseSwgCommand(kernel, properties, controller, command_request)
    {
        ScopedGilLock lock;
        bp::detail::initialize_wrapper(obj, this);
    }

    const std::shared_ptr<ObjectController>& GetController() const
    {        
        ScopedGilLock lock;
        try 
        {
            bp::override get_controller = this->get_override("GetController");
            if (get_controller)
            {
                return get_controller().as<const std::shared_ptr<ObjectController>&>();
            }
        }
        catch(bp::error_already_set& /*e*/)
        {
            PyErr_Print();
        }

        return this->BaseSwgCommand::GetController();
    }

    void Setup()
    {
        ScopedGilLock lock;
        this->get_override("Setup")();
    }

    bool Validate()
    {
        ScopedGilLock lock;
        return this->get_override("Validate")();
    }

    void Run()
    {
        ScopedGilLock lock;
        this->get_override("Run")();
    }
};

void swganh::command::ExportCommand()
{
    bp::class_<CommandInterface, CommandWrapper, boost::noncopyable>("CommandInterface", bp::no_init)
        .def("GetController", bp::pure_virtual(&CommandWrapper::GetController), bp::return_internal_reference<>())
        .def("Setup", bp::pure_virtual(&CommandWrapper::Setup))
        .def("Validate", bp::pure_virtual(&CommandWrapper::Validate))
        .def("Run", bp::pure_virtual(&CommandWrapper::Run))
    ;
    
    bp::class_<BaseSwgCommand, BaseSwgCommandWrapper, bp::bases<CommandInterface>, boost::noncopyable>
        ("BaseSwgCommand", bp::init<swganh::app::SwganhKernel*, const CommandProperties&, const std::shared_ptr<object::ObjectController>&, const swganh::messages::controllers::CommandQueueEnqueue&>())
        .def("GetController", &BaseSwgCommandWrapper::GetController, bp::return_internal_reference<>())
    ;
}
