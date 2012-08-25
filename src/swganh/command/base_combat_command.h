// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "base_swg_command.h"

namespace swganh {
namespace command {

    class BaseCombatCommand : public BaseSwgCommand, public std::enable_shared_from_this<BaseCombatCommand>
    {
    public:
        BaseCombatCommand(
            swganh::app::SwganhKernel* kernel,
            const CommandProperties& properties);

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
