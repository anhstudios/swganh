// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "command_service_binding.h"

#include <boost/python.hpp>

#include "command_service_interface.h"

namespace bp = boost::python;

void swganh::command::ExportCommandService()
{
    bp::class_<CommandServiceInterface, boost::noncopyable>("CommandService", bp::no_init)
        .def("CreateCommand", &CommandServiceInterface::CreateCommand)
        .def("EnqueueCommand", &CommandServiceInterface::EnqueueCommand)
        .def("SetDefaultCommand", &CommandServiceInterface::SetDefaultCommand)
        .def("HasDefaultCommand", &CommandServiceInterface::HasDefaultCommand)
        .def("ClearDefaultCommand", &CommandServiceInterface::ClearDefaultCommand)
        ;
}
