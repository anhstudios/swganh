// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_binding.h"

#include "anh/python_shared_ptr.h"
#include <boost/python.hpp>

#include "base_swg_command.h"
#include "command_interface.h"

namespace bp = boost::python;
using swganh::command::BaseSwgCommand;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;

struct CommandWrapper : CommandInterface, bp::wrapper<CommandInterface>
{    
    const std::shared_ptr<swganh::object::ObjectController>& GetController() const
    {         
        return this->get_override("GetController")().as<const std::shared_ptr<swganh::object::ObjectController>&>();
    }

    void Setup()
    {
        this->get_override("Setup")();
    }

    bool Validate()
    {
        return this->get_override("Validate")();
    }

    void Run()
    {
        this->get_override("Run")();
    }
};

struct BaseSwgCommandWrapper : BaseSwgCommand, bp::wrapper<BaseSwgCommand>
{
    BaseSwgCommandWrapper(
        PyObject* obj,
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties,
        const std::shared_ptr<swganh::object::ObjectController>& controller,
        const swganh::messages::controllers::CommandQueueEnqueue& command_request)
        : BaseSwgCommand(kernel, properties, controller, command_request)
    {
        bp::detail::initialize_wrapper(obj, this);
    }

    const std::shared_ptr<swganh::object::ObjectController>& GetController() const
    {         
        return this->get_override("GetController")().as<const std::shared_ptr<swganh::object::ObjectController>&>();
    }

    void Setup()
    {
        this->get_override("Setup")();
    }

    bool Validate()
    {
        return this->get_override("Validate")();
    }

    void Run()
    {
        this->get_override("Run")();
    }
};

void swganh::command::ExportCommand()
{
    bp::class_<CommandInterface, std::unique_ptr<CommandWrapper>, boost::noncopyable>("CommandInterface", bp::no_init)
        .def("GetController", bp::pure_virtual(&CommandWrapper::GetController), bp::return_internal_reference<>())
        .def("Setup", bp::pure_virtual(&CommandWrapper::Setup))
        .def("Validate", bp::pure_virtual(&CommandWrapper::Validate))
        .def("Run", bp::pure_virtual(&CommandWrapper::Run))
    ;
    
    bp::class_<BaseSwgCommand, std::unique_ptr<BaseSwgCommandWrapper>, bp::bases<CommandInterface>, boost::noncopyable>
        ("BaseSwgCommand", bp::init<swganh::app::SwganhKernel*, const CommandProperties&, const std::shared_ptr<object::ObjectController>&, const swganh::messages::controllers::CommandQueueEnqueue&>())
    ;
}
