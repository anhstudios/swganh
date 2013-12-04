// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "GameSystems_service.h"

#include <memory>
#include "swganh/logger.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/player/player.h"

#include <swganh/service/service_manager.h>
#include <swganh_core/static/static_service.h>
#include <swganh_core/simulation/simulation_service.h>
#include <swganh_core/connection/connection_service_interface.h>
#include <swganh_core/connection/connection_client_interface.h>
#include <swganh_core/object/creature/skill_mod.h>

using namespace swganh::service;
using namespace swganh::object;
using namespace swganh::app;
using namespace swganh::statics;
using namespace swganh::gamesystems;

using swganh::connection::ConnectionServiceInterface;
using swganh::service::ServiceDescription;
using swganh::simulation::SimulationService;
using swganh::connection::ConnectionClientInterface;
//using swganh::messages::MapLocation;
//using swganh::messages::GetMapLocationsRequestMessage;
//using swganh::messages::GetMapLocationsResponseMessage;

GameSystemsService::GameSystemsService(swganh::app::SwganhKernel* kernel)
: kernel_(kernel)
{

    SetServiceDescription(ServiceDescription(
                              "Gamesystems Service",
                              "gamesystems",
                              "0.1",
                              "127.0.0.1",
                              0,
                              0,
                              0));
}

GameSystemsService::~GameSystemsService()
{}

void GameSystemsService::Initialize()
{
	simulation_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");
    database_manager_ = kernel_->GetDatabaseManager();
	equipment_service_ = kernel_->GetServiceManager()->GetService<swganh::equipment::EquipmentServiceInterface>("EquipmentService");

	//command_service_ = kernel_->GetServiceManager()->GetService<swganh::command::CommandServiceInterface>("CommandService");
    //simulation_service_ = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");
    
}

void GameSystemsService::Startup()
{

// Register message handler.
    auto connection_service = kernel_->GetServiceManager()->GetService<ConnectionServiceInterface>("ConnectionService");
    //connection_service->RegisterMessageHandler(&GameSystemsService::HandleRequestMapLocationsMessage, this);
}

void GameSystemsService::GrantSkill(const std::shared_ptr<swganh::object::Creature>& creature, const std::string skill_name)
{
	if(creature->HasSkill(skill_name))	{
		//the creature hasnt got the skill
		DLOG(error) << "SkillManager::GrantSkill :" << creature->GetObjectId() << "already owns skill " << skill_name << " no removal!";
		return;
	}
	
	//look up the skill in the static data
	static_		= kernel_->GetServiceManager()->GetService<StaticService>("StaticService");
	auto static_skills = static_->GetStaticSkillData(skill_name);
	if(static_skills == nullptr)	{
		//we dont know of such a skill :( ideally that shouldnt happen though
		DLOG(error) << "SkillManager::GrantSkill couldnt grant unknown skill : " << skill_name;
		return;
	}

	DLOG(error) << "SkillManager::GrantSkill  : " << skill_name;
	creature->AddSkill(skill_name);

	//iterate through the mods and grant/modify them
	auto mod_map = static_skills->skill_mods_;
	auto mod_itr = mod_map.begin();
	while(mod_itr != mod_map.end())	{
			
		//we now need to check whether we add the mod or just manipulate the value
		
		if(creature->HasSkillMod(mod_itr->first))	{
			//just adjust the value
			auto mod = creature->GetSkillMod(mod_itr->first);
			uint32_t characterModValue	= 	mod.base;
			uint32_t skillModValue		=	mod_itr->second;
				
			if(mod.base != (characterModValue + skillModValue))	{
				mod.base = characterModValue + skillModValue;
				creature->SetSkillMod(mod);
				DLOG(error) << "SkillManager::UpdateSkillMod  : " << mod_itr->first;
			}
			else
				LOG (error) << "skillmod " << mod_itr->first << "didnt change";
				
		}
		// no in this case we need to add it
		else {
			//this will send the relevant deltas to the player
			DLOG(error) << "SkillManager::GrantSkillMod  : " << mod_itr->first;
			creature->AddSkillMod(SkillMod(mod_itr->first, mod_itr->second, 0));
		}
		
		mod_itr ++;

	}
	
	//now skillcommands
	auto cmd_map = static_skills->commands_;
	auto cmd_iter = cmd_map.begin();
	
	auto player = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(creature, "ghost"));

	while(cmd_iter != cmd_map.end())
	{
		//check whether the skillcommand is granted by any other skill
		if(!player->HasSkillCommand((*cmd_iter))) {
			//if not add the skillcommand
			player->AddSkillCommand((*cmd_iter));
			DLOG(error) << "SkillManager::AddSkillCommand  : " << *cmd_iter;
		}

		cmd_iter ++;
	}
	
}

void GameSystemsService::DropSkill(const std::shared_ptr<swganh::object::Creature>& creature, const std::string skill_name)
{

	if(!creature->HasSkill(skill_name))	{
		//the creature hasnt got the skill
		DLOG(error) << "SkillManager::DropSkill :" << creature->GetObjectId() << "doesnt own skill " << skill_name << " no removal!";
		return;
	}
	
	static_		= kernel_->GetServiceManager()->GetService<StaticService>("StaticService");
	//look up the skill in the static data
	auto static_skills = static_->GetStaticSkillData(skill_name);
	if(static_skills == nullptr)	{
		//we dont know of such a skill :( ideally that shouldnt happen though
		DLOG(error) << "SkillManager::DropSkill couldnt drop unknown skill : " << skill_name;
		return;
	}
		
	//remove the skill
	creature->RemoveSkill(skill_name);
	DLOG(error) << "SkillManager::RemoveSkill  : " << skill_name;
	
	//iterate through the mods and remove them
	auto mod_map = static_skills->skill_mods_;
	auto mod_itr = mod_map.begin();
	while(mod_itr != mod_map.end())	{

		if(!creature->HasSkillMod(mod_itr->first))	{
			break;
		}

		//we now need to check whether we remove the mod or just manipulate the value
		auto mod = creature->GetSkillMod(mod_itr->first);

		//uint32_t characterModValue	= 	mod.modifier;
		uint32_t characterModValue	= 	mod.base;
		uint32_t skillModValue		=	mod_itr->second;
			
		//dont remove the mod just alter its value as the mod might be used by other skills, too
		if(characterModValue > skillModValue)	{
			mod.modifier = characterModValue - skillModValue;
			creature->SetSkillMod(mod);
			DLOG(error) << "SkillManager::ChangeSkillMod  : " << mod_itr->first;
		}	
		// no in this case we can remove the mod entirely - it wont be loaded on the next login anyway
		else if(characterModValue == skillModValue)	{
			//this will send the relevant deltas to the player
			creature->RemoveSkillMod(mod_itr->first);
			DLOG(error) << "SkillManager::RemoveSkillMod  : " << mod_itr->first;
		}
		else	{
			//panic
			LOG(error) << "Dropskill skillmodvalues dont add up";
		}
		
		mod_itr ++;

	}
	
	//iterate through the skills commands and remove them unless they get granted by other skills the creature has
	auto cmd_map = static_skills->commands_;
	auto cmd_iter = cmd_map.begin();
	
	auto player = std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(creature, "ghost"));

	while(cmd_iter != cmd_map.end())
	{
		//check whether the skillcommand is granted by any other skill
		if(!static_->CheckSkillCommandDuplicity(creature, skill_name))	{
			//if not remove the skillcommand
			std::string str = (*cmd_iter);
			DLOG(error) << "SkillManager::RemoveSkillCommand  : " << str;
			player->RemoveSkillCommand(str);
		}

		cmd_iter ++;
	}

	
	//handle schematics
	
		
}


/*
int32_t EntertainmentService::GetSlotIdByName(std::string slot_name)
{
    return slot_definitions_->findSlotByName(slot_name);
}

std::string EquipmentService::GetSlotNameById(int32_t slot_id)
{
    return slot_definitions_->entry(slot_id).name;
}

void EquipmentService::ClearSlot(std::shared_ptr<Object> object, std::string slot_name)
{
    if (!object->ClearSlot(slot_definitions_->findSlotByName(slot_name)))
    {
        DLOG(warning) << "Could not find slot with name " << slot_name << " in object " << object->GetObjectId();
    }
}

std::shared_ptr<Object> EquipmentService::GetEquippedObject(std::shared_ptr<Object> object, std::string slot_name)
{
    return object->GetSlotObject(slot_definitions_->findSlotByName(slot_name));
}
*/