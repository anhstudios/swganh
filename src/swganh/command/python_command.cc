
#include "python_command.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"

using namespace boost::python;
using namespace std;
using namespace swganh::command;
using namespace swganh::scripting;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;

PythonCommand::PythonCommand(const CommandProperties& properties)
: script_(PythonScript(properties.script_hook))
{
}

void PythonCommand::operator()(shared_ptr<Creature> actor, shared_ptr<Tangible> target, const swganh::messages::controllers::CommandQueueEnqueue& command_queue_message)
{
	script_.SetContext("actor", boost::python::ptr(actor.get()));
    script_.SetContext("target", boost::python::ptr(target.get()));
    script_.SetContext("command_string", command_queue_message.command_options);
	
	script_.Run();
}