// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include "swganh_core/combat/combat_service_interface.h"
#include "swganh_core/combat/buff_interface.h"
#include "swganh/scripting/utilities.h"

#include <boost/python.hpp>

using namespace swganh::combat;
using namespace boost::python;
using namespace std;
using swganh::scripting::ScopedGilLock;

class BuffWrap : public BuffInterface, wrapper<BuffInterface>
{
public:

	BuffWrap(PyObject* p) 
		: self_(handle<>(borrowed(p)))
	{
		ScopedGilLock lock;
		boost::python::detail::initialize_wrapper(p, this);
	}

	virtual void ApplyBuff(std::shared_ptr<swganh::object::Creature> creature)
	{
		ScopedGilLock lock;
        try 
        {
            if (boost::python::override run = this->get_override("applyBuff"))
            {
                run(creature);
            }
        }
        catch (error_already_set&)
		{
			swganh::scripting::logPythonException();
		}
	}

	virtual void RemoveBuff(std::shared_ptr<swganh::object::Creature> creature)
	{
		ScopedGilLock lock;
        try 
        {
            if (boost::python::override run = this->get_override("removeBuff"))
            {
                run(creature);
            }
        }
        catch (error_already_set&)
		{
			swganh::scripting::logPythonException();
		}
	}

	virtual std::string GetName()
	{
		ScopedGilLock lock;
        try 
        {
            if (boost::python::override run = this->get_override("getName"))
            {
                return run().as<std::string>();
            }
        }
        catch (boost::python::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}

		return "";
	}

	virtual uint32_t GetDuration()
	{
		ScopedGilLock lock;
        try 
        {
            if (boost::python::override run = this->get_override("getDuration"))
            {
                return run();
            }
        }
        catch (boost::python::error_already_set&)
		{
			swganh::scripting::logPythonException();
		}

		return 0;
	}

private:
	object self_;
};

void exportCombatService()
{
    class_<CombatServiceInterface, std::shared_ptr<CombatServiceInterface>, boost::noncopyable>("CombatService", no_init)
        .def("incapacitate", &CombatServiceInterface::SetIncapacitated, "sets the target as incapacitated and sends out appropriate system messages")
        .def("kill", &CombatServiceInterface::SetDead, "Sets the target as dead and sends out the appropriate system messages and begins the cloning process")
        .def("endDuel", &CombatServiceInterface::EndDuel, "Ends the duel between the two players")
        .def("endCombat", &CombatServiceInterface::EndCombat, "Ends combat between the attacker and target")
        ;

	class_<BuffInterface, BuffWrap,  boost::noncopyable>("Buff")
		.def("applyBuff", boost::python::pure_virtual(&BuffInterface::ApplyBuff))
		.def("removeBuff", boost::python::pure_virtual(&BuffInterface::RemoveBuff))
		.def("getName", boost::python::pure_virtual(&BuffInterface::GetName), "Gets or sets the name of this buff")
		.def("getDuration", boost::python::pure_virtual(&BuffInterface::GetDuration))
		;
}
