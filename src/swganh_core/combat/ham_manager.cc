
#ifndef WIN32
#include <Python.h>
#endif

#include "ham_manager.h"

#include <boost/asio/placeholders.hpp>

#include "swganh/event_dispatcher.h"
#include "swganh/logger.h"
#include "swganh/scripting/python_script.h"
#include "swganh/scripting/python_instance_creator.h"

#include "swganh/service/service_manager.h"

#include "swganh_core/combat/ham_events.h"
#include "swganh/observer/observer_interface.h"

#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core\object\creature\posture.h"
#include "swganh_core\object\creature\stat_index.h"

#include "swganh_core/messages/controllers/combat_spam_message.h"

using namespace swganh::scripting;
using namespace boost::posix_time;
using namespace swganh::app;
using namespace swganh::creature;
using namespace swganh::combat;
using namespace swganh::equipment;
using namespace swganh::messages::controllers;
using namespace std;

using swganh::scripting::PythonInstanceCreator;

HamManager::HamManager(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel)
    , timer_(kernel_->GetCpuThreadPool())
{}

void HamManager::Start()
{
	equipment_service_ = kernel_->GetServiceManager()->GetService<swganh::equipment::EquipmentServiceInterface>("EquipmentService");
    //Setup regeneration
	//add us to the regeneration queue
	

    //Start the timer
    timer_.expires_from_now(boost::posix_time::seconds(1));
    timer_.async_wait(boost::bind(&HamManager::handleTick_, this, boost::asio::placeholders::error));
}




uint32_t HamManager::regenerationModifierForce_(std::shared_ptr<swganh::object::Creature> creature)
{

	swganh::object::Posture posture = creature->GetPosture();

	float modifier;
	uint32_t result;
	uint32_t mHealthRegenDivider = 50;

	//todo either script it or get some sort of configuration manager
	switch(posture)	{
		case swganh::object::CROUCHED : modifier = 1.25f;	break;
		case swganh::object::LYING_DOWN :
		case swganh::object::SITTING : modifier = 1.75f;	break;
		case swganh::object::INCAPACITATED : 
		case swganh::object::DEAD : modifier = 0.0f;	break;
		case swganh::object::KNOCKED_DOWN : modifier = 0.75f;	break;
	}

	result = (uint32_t)(25.0f * modifier);

	// Test for creatures
	auto player = std::static_pointer_cast<swganh::object::Player>(equipment_service_->GetEquippedObject(creature, "ghost"));
	if (player)	{
		return result;
	}
	

	return((result/10)+1);
}


uint32_t  HamManager::regenerationModifier_(std::shared_ptr<swganh::object::Creature> creature, uint16_t mainstatIndex)
{
	swganh::object::Posture posture = creature->GetPosture();

	float modifier;
	uint32_t result;
	uint32_t mHealthRegenDivider = 50;

	//todo either script it or get some sort of configuration manager
	switch(posture)	{
		case swganh::object::CROUCHED : modifier = 1.25f;	break;
		case swganh::object::LYING_DOWN :
		case swganh::object::SITTING : modifier = 1.75f;	break;
		case swganh::object::INCAPACITATED : 
		case swganh::object::DEAD : modifier = 0.0f;	break;
		case swganh::object::KNOCKED_DOWN : modifier = 0.75f;	break;
	}

	//mainstatindex+2 is constitution for health; willpower for mind and STAMINA for action
	result = (uint32_t)(creature->GetStatCurrent(mainstatIndex+2) / mHealthRegenDivider * modifier);

	// Test for creatures
	auto player = std::static_pointer_cast<swganh::object::Player>(equipment_service_->GetEquippedObject(creature, "ghost"));
	if (player)	{
		return result;
	}

	return((result/10)+1);
}

void  HamManager::test(uint32_t test)
{

}

int32_t HamManager::UpdateCurrentHitpoints(const std::shared_ptr<swganh::object::Creature> creature, const uint16_t statIndex, const int32_t update)
{
	int32_t currentHam = creature->GetStatCurrent(statIndex); 
	int32_t u = update;

	//sanity check for range
	if((update > 0) && (currentHam >= creature->GetStatMax(statIndex)))	{
		return currentHam;
	}

	//never go below zero
	if((update < 0) && (currentHam += update) < 0)	{
		u = 0-currentHam;
	}

	//No Nonsense In case we get damaged and already are at zero the update will be canceled
	//In case we get spammed with zero updates we will break here
	if(u == 0)	{
		return currentHam;
	}

	currentHam += u;
	creature->SetStatCurrent(statIndex, currentHam);

	//set us on the regeneration timer in case were not already on it
	if(update < 0)	{
		uint64_t id = creature->GetObjectId();
		auto find_itr = reg_.find(id);
		if(find_itr != reg_.end())
		{
			ptime next_time = second_clock::local_time() + seconds(1);
			reg_.insert(std::make_pair(id,std::make_pair(next_time , creature)));
		}        
	}
	
	return currentHam;

	/*
	CombatSpamMessage spam;
    spam.damage = update;
    spam.file = "cbt_spam";
    

    attacker->NotifyObservers(&spam);
	
	*/

}

int32_t  HamManager::ApplyWound(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, int32_t update)
{
	//always check against current hitpoints not the theoretical max
	int32_t currentHam = creature->GetStatCurrent(statIndex); 
	int32_t currentWound = creature->GetStatWound(statIndex); 
	
	//never let us get below 1 health because of wounds
	//otherwise we wont be able to UN incapacitate
	if(update >= currentHam -1)	{
		update = currentHam -1;
	}

	//No neg wound application
	//No Nonsense Updates otherwise the client desyncs
	if(update <= 0)	{
		return 0;
	}

	creature->AddStatWound(statIndex,update);

	//recalculate Hitpoints
	int32_t newHam = creature->GetStatMax(statIndex) - creature->GetStatEncumberance(statIndex) - creature->GetStatWound(statIndex);

	//and adapt them if necessary
	if((currentHam > newHam) && (newHam >0))	{
		creature->SetStatCurrent(statIndex,newHam);
	}
	
	//combatspam
	CombatSpamMessage spam;
    spam.damage = update;
    spam.file = "cbt_spam";
	spam.text = "wounded";
	creature->GetController()->Notify(&spam);

	return update;
}

int32_t HamManager::RemoveWound(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, int32_t update)
{
	//always check against current hitpoints not the theoretical max
	//int32_t currentHam = creature->GetStatCurrent(statIndex); 
	int32_t currentWound = creature->GetStatWound(statIndex); 
	
	//never let us get below 0 Wound
	if(update > currentWound)	{
		update = currentWound;
	}

	//No neg wound application
	//No Nonsense Updates otherwise the client desyncs
	if(update <= 0)	{
		return 0;
	}

	creature->DeductStatWound(statIndex,update);
		
	//combatspam
	/*CombatSpamMessage spam;
    spam.damage = update;
    spam.file = "cbt_spam";
	spam.text = "wounded";
	creature->GetController()->Notify(&spam);
	*/

	return update;
	
}

bool HamManager::ApplyHamCosts(std::shared_ptr<swganh::object::Creature> creature, uint32_t healthCost, uint32_t actionCost, uint32_t mindCost)
{
	//always check against current hitpoints not the theoretical max
	int32_t currentHealth = creature->GetStatCurrent(0); 
	int32_t currentAction = creature->GetStatCurrent(3); 
	int32_t currentMind = creature->GetStatCurrent(6); 
	
	
	//never let us get below 1 we wont incapacitate over playing music lol
	if((healthCost >= (uint32_t) currentHealth) || (actionCost >= (uint32_t) currentAction) || (mindCost >= (uint32_t) currentMind))	{
		return false;
	}

	//No neg cost application
	//No Nonsense Updates otherwise the client desyncs
	if((healthCost == 0) && (actionCost == 0) && (mindCost == 0))	{
		return false;
	}

	currentHealth	-= healthCost;
	currentAction	-= actionCost;
	currentMind		-= mindCost;

	if(healthCost > 0)	{
		creature->SetStatCurrent(0, currentHealth);
	}

	if(actionCost >0)	{
		creature->SetStatCurrent(3, currentAction);
	}

	if(mindCost >0)	{
		creature->SetStatCurrent(6, currentMind);
	}

	//add us to regeneration
	uint64_t id = creature->GetObjectId();
	auto find_itr = reg_.find(id);
	if(find_itr != reg_.end())
	{
		ptime next_time = second_clock::local_time() + seconds(1);
		reg_.insert(std::make_pair(id,std::make_pair(next_time , creature)));
	}        
	
	return true;
	//combatspam
	/*CombatSpamMessage spam;
    spam.damage = update;
    spam.file = "cbt_spam";
	spam.text = "wounded";
	creature->GetController()->Notify(&spam);
	*/
	
}

bool HamManager::ApplyHamCost(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, uint32_t cost)
{
	//always check against current hitpoints not the theoretical max
	int32_t currentHam = creature->GetStatCurrent(statIndex); 
	
	
	//never let us get below 1 we wont incapacitate over playing music lol
	if(cost >= (uint32_t) currentHam)	{
		return false;
	}

	//No neg cost application
	//No Nonsense Updates otherwise the client desyncs
	if(cost <= 0)	{
		return 0;
	}

	currentHam -= cost;
	creature->SetStatCurrent(statIndex, currentHam);

	//add us to regeneration
	uint64_t id = creature->GetObjectId();
	auto find_itr = reg_.find(id);
	if(find_itr != reg_.end())
	{
		ptime next_time = second_clock::local_time() + seconds(1);
		reg_.insert(std::make_pair(id,std::make_pair(next_time , creature)));
	}        
	
	return true;
	//combatspam
	/*CombatSpamMessage spam;
    spam.damage = update;
    spam.file = "cbt_spam";
	spam.text = "wounded";
	creature->GetController()->Notify(&spam);
	*/
	
}

bool HamManager::regenerate_(std::shared_ptr<swganh::object::Creature> creature)
{
	bool done = true;
	//regeneration only for health action mind
	for(uint16_t mainstatIndex = swganh::object::MAINHEALTH; mainstatIndex <= swganh::object::MAINMIND;mainstatIndex++){
		int32_t maxPool = creature->GetStatMax(mainstatIndex);
		if(creature->GetStatCurrent(mainstatIndex) < maxPool ){
			int32_t newPool = UpdateCurrentHitpoints(creature, mainstatIndex, regenerationModifier_(creature, mainstatIndex));
			if (newPool < maxPool)	{
				done = false;
			}
		}
	}

	return done;
	
}

void HamManager::handleTick_(const boost::system::error_code& e)
{
    {
        ptime current_time = second_clock::local_time();

        boost::lock_guard<boost::mutex> lock(mutex_);
		auto iterator = reg_.begin();

		for(iterator = reg_.begin();iterator != reg_.end();iterator++)
		{
			if(current_time > iterator->second.first)	{
				if(regenerate_(iterator->second.second)){
					reg_.erase(iterator);
				}
			}
		}

		
    }
    timer_.expires_from_now(boost::posix_time::seconds(1));
    timer_.async_wait(boost::bind(&HamManager::handleTick_, this, boost::asio::placeholders::error));
}