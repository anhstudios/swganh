
#include "python_command.h"

using namespace boost::python;
using namespace std;
using namespace swganh::scripting;

PythonCommand::PythonCommand(const std::string& filename)
: script_(new PythonScript(filename))
{}

void PythonCommand::operator()(uint32_t object_id, uint32_t target_id, std::wstring command_string)
{
    script_->SetContext("object", object_id);
    script_->SetContext("target", target_id);
    script_->SetContext("command_string", command_string);

    script_->Run();
}
