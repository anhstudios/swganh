#include "python_command.h"

#include <boost/python.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/tangible/tangible.h"

#include "swganh/scripting/python_event.h"

using namespace boost::python;
using namespace std;
using namespace swganh::command;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::object::tangible;

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

        ::object py_command = boost::python::import(name.c_str());
        ::object command_class = py_command.attr((name + "Command").c_str())();
        if (command_class != NULL)
        {
            boost::shared_ptr<Command> command_ptr = extract<boost::shared_ptr<Command>>(command_class);
            command = std::shared_ptr<Command>(command_ptr.get(), [command_class](Command*){});
        }
    }
    catch(...)
    {
        // mute errors
        //PyErr_Clear();
    }

    return command;
}