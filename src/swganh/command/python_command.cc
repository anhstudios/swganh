#include "python_command.h"
#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"

#include "swganh/scripting/python_event.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace boost::python;
using namespace std;
using namespace swganh::command;
using namespace swganh::scripting;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;

PythonCommand::PythonCommand(const shared_ptr<Command>& properties, const std::string& script_path)
: script_(PythonScript(script_path))
{
}
std::shared_ptr<Command> PythonCommand::LoadCommand(const std::string& name, const std::string& full_path)
{
    std::shared_ptr<Command> command;
    try {
        // Don't try if the file doesn't exist
        if (!boost::filesystem::exists(full_path))
            return command;
        // append command dir
        string path = full_path.substr(0, full_path.find_last_of("/"));
        string py_path = "import sys; sys.path.append('.');sys.path.append('" + path + "');";
        Py_Initialize();
        PyRun_SimpleString(py_path.c_str());
        boost::python::object mm = boost::python::import("__main__");
        boost::python::object mn = mm.attr("__dict__");

        boost::python::exec(("import " + name).c_str(), mn);

        boost::python::object py_object = boost::python::eval((name + "." + name + "Command()").c_str(), mn);
        
        extract<Command*> comm(py_object);
        if (comm.check())
        {
            command = std::shared_ptr<Command>(comm());
            command->Setup();
        }
    }
    catch(...)
    {
        // mute errors
        //PyErr_Clear();
    }

    return command;
}

void PythonCommand::operator()(swganh::app::SwganhKernel* kernel, shared_ptr<Creature> actor, shared_ptr<Tangible> target, const swganh::messages::controllers::CommandQueueEnqueue& command_queue_message)
{
    shared_ptr<Creature> creature = nullptr;
    if (target && target->GetType() == Creature::type)
        creature = static_pointer_cast<Creature>(target);

    // Setup Python Event
    auto python_event = make_shared<PythonEvent>();
    python_event->globals = script_.GetGlobals();
    
    script_.SetContext("kernel", boost::python::ptr(kernel));
	script_.SetContext("actor", actor);
    script_.SetContext("target", target);
    script_.SetContext("creature_target", creature);
    script_.SetContext("command_string", command_queue_message.command_options);
    script_.SetContext("event", python_event);
	
	script_.Run();
}