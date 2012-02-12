
#ifndef SWGANH_COMMAND_PYTHON_COMMAND_H_
#define SWGANH_COMMAND_PYTHON_COMMAND_H_

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/scripting/python_script.h"

#include "command_properties.h"

namespace swganh {
namespace object {
	class Object;
namespace creature {
	class Creature;
}}
namespace command {

    class PythonCommand
    {
    public:
        PythonCommand(const CommandProperties& command_properties);

        void operator()(std::shared_ptr<swganh::object::creature::Creature> actor, std::shared_ptr<swganh::object::Object> target, std::wstring command_string);

    private:
        swganh::scripting::PythonScript script_;
    };


}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_PYTHON_COMMAND_H_
