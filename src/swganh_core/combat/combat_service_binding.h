// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#ifndef WIN32
#include <Python.h>
#endif

#include "swganh/scripting/python_shared_ptr.h"
#include "swganh_core/combat/combat_service_interface.h"
//#include "swganh_core/combat/combat_service.h"
#include "swganh_core/combat/buff_interface.h"
#include "swganh_core/combat/ham_interface.h"
#include "swganh/scripting/utilities.h"

#include <boost/python.hpp>

using namespace swganh::combat;
//using namespace swganh::creature;
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

	typedef bool	(HamInterface::*ApplyHamCostBinding)(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, uint32_t cost);
	typedef int32_t (HamInterface::*UpdateCurrentHitpointsBinding)(const std::shared_ptr<swganh::object::Creature> creature, const uint16_t statIndex, const int32_t update);
	typedef int32_t	(HamInterface::*ApplyWoundBinding)(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, int32_t update);
	typedef HamInterface*	(CombatServiceInterface::*GetHamManager);
	typedef bool	(HamInterface::*ApplyHamCostsBinding)(std::shared_ptr<swganh::object::Creature> creature, uint32_t healthCost, uint32_t actionCost, uint32_t mindCost);



    class_<CombatServiceInterface, std::shared_ptr<CombatServiceInterface>, boost::noncopyable>("CombatService", no_init)
    .def("incapacitate", &CombatServiceInterface::SetIncapacitated, "sets the target as incapacitated and sends out appropriate system messages")
    .def("kill", &CombatServiceInterface::SetDead, "Sets the target as dead and sends out the appropriate system messages and begins the cloning process")
    .def("endDuel", &CombatServiceInterface::EndDuel, "Ends the duel between the two players")
    .def("endCombat", &CombatServiceInterface::EndCombat, "Ends combat between the attacker and target")
	.def("getHamManager", &CombatServiceInterface::GetHamManager,return_value_policy<reference_existing_object>(), "Gets us the Ham Manager to Manipulate Ham")
	;

	

    class_<BuffInterface, BuffWrap,  boost::noncopyable>("Buff")
    .def("applyBuff", boost::python::pure_virtual(&BuffInterface::ApplyBuff))
    .def("removeBuff", boost::python::pure_virtual(&BuffInterface::RemoveBuff))
    .def("getName", boost::python::pure_virtual(&BuffInterface::GetName), "Gets or sets the name of this buff")
    .def("getDuration", boost::python::pure_virtual(&BuffInterface::GetDuration))
    ;

	class_<HamInterface, HamInterface,  boost::noncopyable>("Ham", no_init)
    .def("updateCurrentHitpoints",  UpdateCurrentHitpointsBinding(&HamInterface::UpdateCurrentHitpoints),"updates the hitpoints with given value after performing sanity checks. ")
	.def("applyWound", ApplyWoundBinding(&HamInterface::ApplyWound), "applies a wound after sanity checks and recalculates ham")
    .def("removeWound", &HamInterface::RemoveWound, "removes a wound and puts us on the regeneration timer")
    .def("applyHamCost", ApplyHamCostBinding(&HamInterface::ApplyHamCost), "applies the cost of whatever to ham, returns true if succesfull, bails out with false")
	.def("applyHamCosts", ApplyHamCostsBinding(&HamInterface::ApplyHamCosts), "applies costs to several Ham Elements (Health, Action, Mind), returns true if succesfull, bails out with false.")
    ;
}
