
#include "python_command.h"
#include "swganh/object/object.h"

using namespace boost::python;
using namespace std;
using namespace swganh::command;
using namespace swganh::scripting;

PythonCommand::PythonCommand(const CommandProperties& properties)
: script_(PythonScript(properties.script_hook))
{
}

void PythonCommand::operator()(std::shared_ptr<swganh::object::Object> object, uint32_t object_id, uint32_t target_id, std::wstring command_string)
{
	script_.SetContext("actor", boost::python::ptr(object.get()));
    script_.SetContext("object_id", object_id);
    script_.SetContext("target", target_id);
    script_.SetContext("command_string", command_string);
	
	script_.Run();
}