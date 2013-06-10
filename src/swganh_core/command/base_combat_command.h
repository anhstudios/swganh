// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "base_swg_command.h"

namespace swganh {
namespace combat {
	struct CombatData;
}
namespace command {
	
    class BaseCombatCommand : public BaseSwgCommand, public std::enable_shared_from_this<BaseCombatCommand>
    {
    public:
        virtual void Run();

		std::shared_ptr<swganh::combat::CombatData> combat_data;
    };

}}
