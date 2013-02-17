// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{
	class Creature;
}

namespace combat
{
	struct BuffEvent : swganh::BaseEvent
	{
		BuffEvent(swganh::EventType type, 
			std::shared_ptr<swganh::object::Creature> object_, 
			swganh::HashString name_,
			uint32_t duration_)
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