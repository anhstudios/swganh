
#include "python_command.h"
#include "swganh/object/creature/creature.h"

using namespace boost::python;
using namespace std;
using namespace swganh::command;
using namespace swganh::scripting;
using namespace swganh::object;
using namespace swganh::object::creature;

PythonCommand::PythonCommand(const CommandProperties& properties)
: script_(PythonScript(properties.script_hook))
{
}

void PythonCommand::operator()(shared_ptr<Creature> actor, shared_ptr<Object> target, std::wstring command_string)
{
	script_.SetContext("actor", boost::python::ptr(actor.get()));
    script_.SetContext("target", boost::python::ptr(target.get()));
    script_.SetContext("command_string", command_string);
	
	script_.Run();
}