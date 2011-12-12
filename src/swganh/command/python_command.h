
#ifndef SWGANH_COMMAND_PYTHON_COMMAND_H_
#define SWGANH_COMMAND_PYTHON_COMMAND_H_

#include <cstdint>
#include <memory>
#include <string>

#include "swganh/scripting/python_script.h"

#include "command_properties.h"

namespace swganh {
namespace command {

    class PythonCommand
    {
    public:
        PythonCommand(const CommandProperties& command_properties);

        void operator()(uint32_t object_id, uint32_t target_id, std::wstring command_string);

    private:
        swganh::scripting::PythonScript script_;
    };

}}  // namespace swganh::command

#endif  // SWGANH_COMMAND_PYTHON_COMMAND_H_
