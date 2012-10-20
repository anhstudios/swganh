 //This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/service/service_interface.h"

namespace swganh {
namespace object {
	class Player;
	class Creature;
}}

namespace swganh {
namespace player {

class PlayerServiceInterface : swganh::service::ServiceInterface
{
public:
	virtual void OnPlayerEnter(std::shared_ptr<swganh::object::Player> player) = 0;

	virtual void OnPlayerExit(std::shared_ptr<swganh::object::Player> player) = 0;

	virtual void SendBankTip(const std::shared_ptr<swganh::object::Creature>& from, const std::shared_ptr<swganh::object::Creature>& to, uint32_t amount) = 0;
	
	virtual void OpenBank(const std::shared_ptr<swganh::object::Creature>& owner) = 0;
};



}}
