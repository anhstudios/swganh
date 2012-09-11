// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/player/player_service_interface.h"

#include <boost/asio.hpp>

#include "swganh/app/swganh_kernel.h"
#include "swganh/event_dispatcher.h"

namespace swganh {
namespace object {
	class ObjectController;
} // object
namespace simulation {
	class SimulationServiceInterface;
}} // swganh::simulation

namespace swganh {
namespace player {

	class PlayerService : public swganh::player::PlayerServiceInterface
	{
	public:
		PlayerService(swganh::app::SwganhKernel* kernel);
		
		void Startup();

		swganh::service::ServiceDescription GetServiceDescription();

		void CleanupPlayerState(std::shared_ptr<swganh::object::Player> player);

		void OnPlayerEnter(std::shared_ptr<swganh::object::Player> player);

		void OnPlayerExit(std::shared_ptr<swganh::object::Player> player);
	private:
		void RemoveClientTimerHandler_(
			const boost::system::error_code& e, 
			std::shared_ptr<boost::asio::deadline_timer> timer, 
			int delay_in_secs, 
			std::shared_ptr<swganh::object::ObjectController> controller);

		swganh::app::SwganhKernel* kernel_;
		swganh::simulation::SimulationServiceInterface* simulation_service_;
		swganh::CallbackId player_selected_callback_;
		swganh::CallbackId player_removed_;
	};

}} // swganh::player
