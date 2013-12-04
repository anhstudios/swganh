// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh/service/service_interface.h>

namespace swganh
{

namespace object	{ class Object; class Creature; }

namespace gamesystems
{
class GameSystemsServiceInterface : public swganh::service::BaseService
{
public:
    virtual ~GameSystemsServiceInterface() {}

	virtual void DropSkill(const std::shared_ptr<swganh::object::Creature>& creature, const std::string skill_name) = 0;

	virtual	void GrantSkill(const std::shared_ptr<swganh::object::Creature>& creature, const std::string skill_name) = 0;

    //virtual int32_t GetSlotIdByName(std::string slot_name) = 0;
    //virtual std::string GetSlotNameById(int32_t slot_id) = 0;

    //virtual void ClearSlot(std::shared_ptr<swganh::object::Object> object, std::string slot_name) = 0;

    

    //virtual std::shared_ptr<swganh::object::Object> GetEquippedObject(
      //  std::shared_ptr<swganh::object::Object> object, std::string slot_name) = 0;
};
}
}
