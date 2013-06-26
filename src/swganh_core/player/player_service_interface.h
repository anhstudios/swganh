 //This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/service/service_interface.h"

namespace swganh {
namespace object {
	class Object;
	class Player;
	class Creature;
}}

namespace swganh {
namespace player {

class PlayerServiceInterface : public swganh::service::BaseService
{
public:
    virtual ~PlayerServiceInterface() {}

	virtual void OnPlayerEnter(std::shared_ptr<swganh::object::Player> player) = 0;

	virtual void OnPlayerExit(std::shared_ptr<swganh::object::Player> player) = 0;

	virtual void SendTip(const std::shared_ptr<swganh::object::Creature>& from, const std::shared_ptr<swganh::object::Creature>& to, uint32_t amount, bool bank = false) = 0;
	
	virtual void OpenContainer(const std::shared_ptr<swganh::object::Creature>& owner, std::shared_ptr<swganh::object::Object> object) = 0;

	virtual bool HasCalledMount(std::shared_ptr<swganh::object::Creature> owner) = 0;

	virtual void StoreAllCalledMounts(std::shared_ptr<swganh::object::Creature> owner) = 0;
	
	virtual void StoreAllCalledObjects(std::shared_ptr<swganh::object::Creature> owner) = 0;

};



}}
