
#ifndef SWGANH_SCRIPTING_PYTHON_COMMAND_H_
#define SWGANH_SCRIPTING_PYTHON_COMMAND_H_

#include <cstdint>
#include <memory>
#include <string>

#include "python_script.h"

namespace swganh {
namespace scripting {

    class PythonCommand
    {
    public:
        PythonCommand(const std::string& filename);

        void operator()(uint32_t object_id, uint32_t target_id, std::wstring command_string);

    private:
        std::unique_ptr<PythonScript> script_;
    };

}}  // namespace swganh::scripting

#endif  // SWGANH_SCRIPTING_PYTHON_COMMAND_H_
