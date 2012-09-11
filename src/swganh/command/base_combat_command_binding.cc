// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "base_combat_command_binding.h"

#include "swganh/python_shared_ptr.h"
#include <boost/python.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/tangible/tangible.h"
#include "swganh/scripting/utilities.h"

#include "base_combat_command.h"

namespace bp = boost::python;
using swganh::command::BaseCombatCommand;
using swganh::command::CommandCallback;
using swganh::command::CommandInterface;
using swganh::command::CommandProperties;
using swganh::scripting::ScopedGilLock;

struct BaseCombatCommandWrapper : BaseCombatCommand, bp::wrapper<BaseCombatCommand>
{
    BaseCombatCommandWrapper(
        PyObject* obj,
        swganh::app::SwganhKernel* kernel,
        const CommandProperties& properties)
        : BaseCombatCommand(kernel, properties)
        , self_(bp::handle<>(bp::borrowed(obj)))
    {
        ScopedGilLock lock;
        bp::detail::initialize_wrapper(obj, this);
    }

    boost::optional<std::shared_ptr<CommandCallback>> Run()
    {
        boost::optional<std::shared_ptr<CommandCallback>> callback;


        ScopedGilLock lock;
        try 
        {
            if (bp::override run = this->get_override("Run"))
            {
                bp::object result = run();

                if (!result.is_none())
                {
                    CommandCallback* obj_pointer = bp::extract<CommandCallback*>(result);
                    callback.reset(std::shared_ptr<CommandCallback>(obj_pointer, [result] (CommandCallback*) {}));
                }
            }
            
            this->BaseCombatCommand::Run();
        }
        catch(bp::error_already_set& /*e*/)
        {
            PyErr_Print();
        }

        return callback;
    }

    template <typename T>
    void ExtractData(boost::python::object& p_object, std::string key, T& extract_value)
    {
        swganh::scripting::ScopedGilLock lock;
        
        try 
        {
            if (PyObject_HasAttrString(self_.ptr(), key.c_str()))
            {
                boost::python::extract<T> tmp_x(p_object.attr(key.c_str()));
                if (tmp_x.check())
                    extract_value = tmp_x();
            }
        }
        catch(bp::error_already_set& /*e*/)
        {
            PyErr_Print();
        }
    }
        
    virtual int GetMinDamage()
    {
        int min_damage = 0;
        ExtractData(self_, "min_damage", min_damage);
        return min_damage;
    }

    virtual int GetMaxDamage()
    {
        int max_damage = 0;
        ExtractData(self_, "max_damage", max_damage);
        return max_damage;
    }

    virtual float GetDamageMultiplier()
    {
        float damage_multiplier = 0.0f;
        ExtractData(self_, "damage_multiplier", damage_multiplier);
        return damage_multiplier;
    }

    virtual int GetAccuracyBonus()
    {
        int accuracy_bonus = 0;
        ExtractData(self_, "accuracy_bonus", accuracy_bonus);
        return accuracy_bonus;
    }

    virtual float GetSpeedMultiplier()
    {
        float speed_multiplier = 0.0f;
        ExtractData(self_, "speed_multiplier", speed_multiplier);
        return speed_multiplier;
    }

    virtual float GetHealthHitChance()
    {
        float health_hit_chance = 0.0f;
        ExtractData(self_, "health_hit_chance", health_hit_chance);
        return health_hit_chance;
    }

    virtual float GetActionHitChance()
    {
        float action_hit_chance = 0.0f;
        ExtractData(self_, "action_hit_chance", action_hit_chance);
        return action_hit_chance;
    }

    virtual float GetMindHitChance()
    {
        float mind_hit_chance = 0.0f;
        ExtractData(self_, "mind_hit_chance", mind_hit_chance);
        return mind_hit_chance;
    }

private:
    bp::object self_;
};

void swganh::command::ExportBaseCombatCommand()
{
    bp::class_<BaseCombatCommand, BaseCombatCommandWrapper, bp::bases<BaseSwgCommand>, boost::noncopyable>
        ("BaseCombatCommand", bp::init<swganh::app::SwganhKernel*, const CommandProperties&>())
        .def("Run", &BaseCombatCommandWrapper::Run)
    ;
}
