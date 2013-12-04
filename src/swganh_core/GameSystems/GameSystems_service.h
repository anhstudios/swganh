// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh_core/gamesystems/GameSystems_service_interface.h>
#include <swganh_core/static/static_service_interface.h>
#include "swganh_core/equipment/equipment_service_interface.h"
#include <swganh_core/object/creature/creature.h>

namespace swganh
{

namespace app		{ class SwganhKernel; }
namespace connection{ class ConnectionClientInterface;} // namespace swganh::connection
namespace database	{ class DatabaseManager; }
namespace object	{ class Object; class Player; class Creature; } // namespace swganh::object
namespace simulation{ class SimulationService; } // namespace
namespace statics	{ class StaticService; }
namespace messages	{ struct ChatInstantMessageToCharacter; } //tell
namespace skill		{ class Skill;}  // namespace

namespace gamesystems
{
/**
* A service that provides wrapping functionality for the container system
* Methods on this interface basically convert slot name to slot id
*/
	class GameSystemsService : public swganh::gamesystems::GameSystemsServiceInterface
{
public:
    /**
    * Creates a new instance
    */
    GameSystemsService(swganh::app::SwganhKernel* kernel);

    ~GameSystemsService();

	virtual void Startup();

    virtual void Initialize();

	/*
     * @brief Drops a skill	
     * updates the relevant Skillmods and commands afterwards
     */
	virtual void DropSkill(const std::shared_ptr<swganh::object::Creature>& creature, const std::string skill_name);

	/*
     * @brief Grants a skill	
     * updates the relevant Skillmods and commands afterwards
     */
	virtual void GrantSkill(const std::shared_ptr<swganh::object::Creature>& creature, const std::string skill_name);

    /**
    * @return the id of a slot by name
    */
    //virtual int32_t GetSlotIdByName(std::string slot_name);

    /**
    * @return the name of a slot by id
    */
    //virtual std::string GetSlotNameById(int32_t slot_id);

    /**
    * Clears a slot for an object by name
    * @param object the object to manipulate
    * @param slot_name the name of the slot to clear
    */
    //virtual void ClearSlot(std::shared_ptr<swganh::object::Object> object, std::string slot_name);

    /**
    * @param object the object to search
    * @param slot_name the name of the slot to return
    * @returns a sub object of object by slot name
    */
    //virtual std::shared_ptr<swganh::object::Object> GetEquippedObject(
    //    std::shared_ptr<swganh::object::Object> object, std::string slot_name);

private:

	//Skill related

	//drop Skill
   /* void _handleWhatever(
        const std::shared_ptr<swganh::connection::ConnectionClientInterface>& client,
        swganh::messages::ChatInstantMessageToCharacter* message);*/

	swganh::app::SwganhKernel* kernel_;
    swganh::simulation::SimulationService* simulation_;
	swganh::statics::StaticService* static_;
    swganh::database::DatabaseManager* database_manager_;
	swganh::equipment::EquipmentServiceInterface* equipment_service_;
    //std::shared_ptr<swganh::tre::SlotDefinitionVisitor> slot_definitions_;
};
}
}