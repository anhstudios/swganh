// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <set>
#include <vector>
//#include <list>
#include <queue>

#include <boost/thread/mutex.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <swganh_core/combat/ham_interface.h>
#include <swganh_core/object/creature/creature.h>
#include <swganh/app/swganh_kernel.h>
#include "swganh_core/equipment/equipment_service_interface.h"

namespace swganh
{

namespace creature
{
class Creature;

namespace player
{
	class Player;
}
}
namespace combat
{
	
namespace equipment
{
class EquipmentServiceInterface;
}

typedef std::pair<boost::posix_time::ptime, std::shared_ptr<swganh::object::Creature>> RegenerationQueueType;
typedef std::set<std::pair<std::shared_ptr<swganh::object::Creature>,boost::posix_time::ptime>> RegenerationQueue;

struct RenerationRateType	{

	uint32_t mHealthRegenRate;
	uint32_t mActionRegenRate;
	uint32_t mMindRegenRate;
	uint32_t mForceRegenRate;

};


class HamManager  :public swganh::combat::HamInterface
{
public:

    HamManager(swganh::app::SwganhKernel* kernel);

    void Start();
	
	void test(uint32_t test);
	
	/** Updates the HamBar (statIndex) with (update)
	*	performs sanity checks on whether an update is necessary 
	*	set creature on regeneration timer in case there is damage
	*	modifies the ham in case of update
	*/
	virtual int32_t		UpdateCurrentHitpoints(const std::shared_ptr<swganh::object::Creature> creature, const uint16_t statIndex, const int32_t update);


	/**	adds a wound to the relevant ham element
	*	performs sanity checks on wound size and updates the hitpoints if necessary
	*/
	virtual int32_t		ApplyWound(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, int32_t update);
	

	//heals a wound of the relevant ham element
	//performs sanity checks on wound size and updates the hitpoints if necessary
	//
	virtual int32_t		RemoveWound(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, int32_t update);


	/**	Applies the stat cost of a command in case it can be executed
	//	returns false if ham cost cant be met
	*/
	virtual bool		ApplyHamCost(std::shared_ptr<swganh::object::Creature> creature, uint16_t statIndex, uint32_t cost);

	

	/**	Applies the (several) stat costs of a command in case it can be executed
	//	returns false if ham costs cant be met
	*/
	virtual bool		ApplyHamCosts(std::shared_ptr<swganh::object::Creature> creature, uint32_t healthCost, uint32_t actionCost, uint32_t mindCost);
	

private:
    void handleTick_(const boost::system::error_code& e);
	bool regenerate_(std::shared_ptr<swganh::object::Creature> creature);
	
	uint32_t 	regenerationModifier_(std::shared_ptr<swganh::object::Creature> creature, uint16_t mainstatIndex);
	uint32_t	regenerationModifierForce_(std::shared_ptr<swganh::object::Creature> creature);
	
	
	

    swganh::app::SwganhKernel * kernel_;

    boost::mutex		mutex_;
	RegenerationQueue	q_;
    //std::map<swganh::HashString, std::shared_ptr<HamInterface>> hams_;
	std::map<uint64_t,RegenerationQueueType> reg_;

    boost::asio::deadline_timer timer_;
	swganh::equipment::EquipmentServiceInterface* equipment_service_;
};

}
}