// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_binding.h"

#include "anh/python_shared_ptr.h"
#include <boost/python.hpp>

#include "command_interface.h"

namespace bp = boost::python;
using swganh::command::CommandInterface;

struct CommandWrapper : CommandInterface, bp::wrapper<CommandInterface>
{    
    void Setup()
    {
        if (bp::override setup_func = this->get_override("Setup"))
        {
            setup_func();
        }

        return CommandInterface::Setup();
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
    bp::class_<CommandInterface, std::shared_ptr<CommandWrapper>, boost::noncopyable>("CommandInterface", bp::no_init)
        .def("Setup", &CommandWrapper::Setup)
        .def("Validate", &CommandWrapper::Validate)
        .def("Run", &CommandWrapper::Run)
    ;
}
