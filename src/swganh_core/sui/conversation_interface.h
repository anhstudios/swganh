// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{

namespace object
{
	class Creature;
}

namespace sui
{
class ConversationInterface
{
public:

	virtual ~ConversationInterface() {}

	virtual void startConversation(std::shared_ptr<swganh::object::Creature>& npc, std::shared_ptr<swganh::object::Creature>& player) = 0;
	
	virtual void stopConversation(std::shared_ptr<swganh::object::Creature>& npc, std::shared_ptr<swganh::object::Creature>& player) = 0;

	virtual void updateConversation(std::shared_ptr<swganh::object::Creature>& npc, std::shared_ptr<swganh::object::Creature>& player, uint32_t option_id) = 0;
};

}
}