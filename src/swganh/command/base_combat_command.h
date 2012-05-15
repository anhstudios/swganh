// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_COMMAND_BASE_COMBAT_COMMAND_H_
#define SWGANH_COMMAND_BASE_COMBAT_COMMAND_H_

#include "base_swg_command.h"

namespace swganh {
namespace command {

    class BaseCombatCommand : public BaseSwgCommand
    {
    public:
        BaseCombatCommand(
            swganh::app::SwganhKernel* kernel,
            const CommandProperties& properties,
            const std::shared_ptr<object::ObjectController>& controller,
            const swganh::messages::controllers::CommandQueueEnqueue& command_request);

        virtual ~BaseCombatCommand();
        
        virtual boost::optional<std::shared_ptr<CommandCallback>> Run();
        
        virtual int GetMinDamage() = 0;
        virtual int GetMaxDamage() = 0;

        virtual float GetDamageMultiplier() = 0;
        virtual int GetAccuracyBonus() = 0;
        virtual float GetSpeedMultiplier() = 0;

        virtual float GetHealthHitChance() = 0;
        virtual float GetActionHitChance() = 0;
        virtual float GetMindHitChance() = 0;
    };

}}

#endif  // SWGANH_COMMAND_BASE_COMBAT_COMMAND_H_
