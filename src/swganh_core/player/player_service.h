// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/player/player_service_interface.h"

#include <boost/asio.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/event_dispatcher.h"

namespace swganh {
namespace equipment {
	class EquipmentService;
}
namespace object {
	class ObjectController;	
} // object
namespace simulation {
	class SimulationServiceInterface;
}} // swganh::simulation

namespace swganh {
namespace player {

/**
* Provides basic functionality of player logout
*/
class PlayerService : public swganh::player::PlayerServiceInterface
{
public:
	/**
	* Creates a new instance
	*/
	PlayerService(swganh::app::SwganhKernel* kernel);
		
    ~PlayerService();

	void Initialize();
	void Startup();

	/**
	* Called when a player enters the game
	*/
	void OnPlayerEnter(std::shared_ptr<swganh::object::Player> player);

	/**
	* Called when a player exits the game
	*/
	void OnPlayerExit(std::shared_ptr<swganh::object::Player> player);

	/**
	 *  Send Tip
	 */
	void SendTip(const std::shared_ptr<swganh::object::Creature>& from, const std::shared_ptr<swganh::object::Creature>& to, uint32_t amount, bool bank = false);

	/**
	 *  Open Container
	 */
	virtual void OpenContainer(const std::shared_ptr<swganh::object::Creature>& owner, std::shared_ptr<swganh::object::Object> object);

	virtual bool HasCalledMount(std::shared_ptr<swganh::object::Creature> owner);

	virtual void StoreAllCalledMounts(std::shared_ptr<swganh::object::Creature> owner);
	
	virtual void StoreAllCalledObjects(std::shared_ptr<swganh::object::Creature> owner);

private:
	void RemoveClientTimerHandler_(
		const boost::system::error_code& e, 
		std::shared_ptr<boost::asio::deadline_timer> timer, 
		int delay_in_secs, 
		std::shared_ptr<swganh::object::ObjectController> controller);

	swganh::app::SwganhKernel* kernel_;
	swganh::equipment::EquipmentService* equipment_service_;
	swganh::simulation::SimulationServiceInterface* simulation_service_;
};

}} // swganh::player
