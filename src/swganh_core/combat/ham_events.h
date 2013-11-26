// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/object/creature/stat_index.h"
#include "swganh/event_dispatcher.h"

namespace swganh
{
namespace object
{
class Creature;
//enum StatIndex;
}

namespace creature
{
struct HamEvent : swganh::BaseEvent
{
    HamEvent(swganh::EventType type,
              std::shared_ptr<swganh::object::Creature> object_,
              swganh::HashString name_,
			  int32_t amount_,
			  swganh::object::StatIndex hambar_)
        : BaseEvent(type)
        , object(object_)
        , name(name_)
		, amount(amount_)
		, HAMBar(hambar_)
    {}

	
    std::shared_ptr<swganh::object::Creature> object;
    swganh::HashString name;
    uint32_t duration;
	int32_t amount;
	swganh::object::StatIndex HAMBar;
};

struct RegenerationEvent : swganh::BaseEvent
{
    RegenerationEvent(swganh::EventType type,
              std::shared_ptr<swganh::object::Creature> object_,
              swganh::HashString name_,
			  uint32_t duration_,
			  swganh::object::StatIndex hambar_)
        : BaseEvent(type)
        , object(object_)
        , name(name_)
		, duration(duration_)
    {}

	
    std::shared_ptr<swganh::object::Creature> object;
    swganh::HashString name;
    uint32_t duration;
};
}
}