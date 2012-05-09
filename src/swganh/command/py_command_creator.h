// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_PY_COMMAND_CREATOR_H_
#define SWGANH_COMMAND_PY_COMMAND_CREATOR_H_

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

    class PyCommandCreator
    {
    public:
        PyCommandCreator(std::string module_name, std::string class_name);

        std::unique_ptr<CommandInterface> operator() (
            swganh::app::SwganhKernel* kernel,
            const CommandProperties& properties,
            const std::shared_ptr<object::ObjectController>& controller,
            const swganh::messages::controllers::CommandQueueEnqueue& command_request);

    private:
        std::string module_name_;
        std::string class_name_;
        
        boost::python::object command_module_;
    };

}}

#endif  // SWGANH_COMMAND_PY_COMMAND_CREATOR_H_
