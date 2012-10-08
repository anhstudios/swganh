// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <memory>
#include <string>

#include <boost/python/object.hpp>

namespace swganh {
namespace app {
    class SwganhKernel;
}

namespace messages {
namespace controllers {
    class CommandQueueEnqueue;
}}

namespace object {
    class ObjectController;
}

namespace command {

    class CommandInterface;
    struct CommandProperties;

    class PythonCommandCreator
    {
    public:
        PythonCommandCreator(std::string module_name, std::string class_name);

        std::shared_ptr<CommandInterface> operator() (
            swganh::app::SwganhKernel* kernel,
            const CommandProperties& properties);

    private:
        std::string module_name_;
        std::string class_name_;
        
        boost::python::object command_module_;
    };

}}
