// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "player_service.h"

#include "swganh/logger.h"
#include "swganh/event_dispatcher.h"
#include "swganh/service/service_description.h"
#include "swganh/service/service_manager.h"
#include "swganh/connection/connection_client_interface.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/object/object_controller.h"
#include "swganh/simulation/simulation_service_interface.h"

using namespace std;
using namespace swganh;
using namespace swganh::service;
using namespace swganh::player;
using namespace swganh::object;


ServiceDescription PlayerService::GetServiceDescription()
{
    ServiceDescription service_description(
        "PlayerService",
        "player",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0);

    return service_description;
}

PlayerService::PlayerService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{

	player_selected_callback_ = kernel_->GetEventDispatcher()->Subscribe(
		"Simulation::PlayerSelected",
		[this] (shared_ptr<EventInterface> incoming_event)
	{
		const auto& player = static_pointer_cast<ValueEvent<shared_ptr<Player>>>(incoming_event)->Get();
		OnPlayerEnter(player);
	});

	player_removed_ = kernel_->GetEventDispatcher()->Subscribe(
		"Connection::PlayerRemoved",
		[this] (shared_ptr<EventInterface> incoming_event)
	{
		const auto& player = static_pointer_cast<ValueEvent<shared_ptr<Player>>>(incoming_event)->Get();
		OnPlayerExit(player);
	});
}

void PlayerService::Startup()
{
	simulation_service_ = kernel_->GetServiceManager()->
		GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");
}

void PlayerService::CleanupPlayerState(shared_ptr<swganh::object::Player> player)
{

}

void PlayerService::OnPlayerEnter(shared_ptr<swganh::object::Player> player)
{
    if (player)
    {
	    player->ClearStatusFlags();
    }
}

void PlayerService::OnPlayerExit(shared_ptr<swganh::object::Player> player)
{
    if (player)
    {
	    player->ClearStatusFlags();
	    player->AddStatusFlag(swganh::object::LD);
	    // set a timer to 30 seconds to destroy the object, unless logged back in.
        auto deadline_timer = std::make_shared<boost::asio::deadline_timer>(kernel_->GetIoService(), boost::posix_time::seconds(30));
		auto parent = std::static_pointer_cast<swganh::object::Object>(player->GetContainer());

		auto object_controller = std::static_pointer_cast<swganh::object::ObjectController>(parent->GetController());
		if(object_controller != nullptr)
		{
			deadline_timer->async_wait(boost::bind(&PlayerService::RemoveClientTimerHandler_, this, boost::asio::placeholders::error, deadline_timer, 30, object_controller));
		}
    }
}

void PlayerService::RemoveClientTimerHandler_(
    const boost::system::error_code& e,
    shared_ptr<boost::asio::deadline_timer> timer,
    int delay_in_secs,
    shared_ptr<swganh::object::ObjectController> controller)
{
    if (controller)
    {
        // destroy if they haven't reconnected
        if (controller->GetRemoteClient() == nullptr || !controller->GetRemoteClient()->connected())
        {
            auto object = controller->GetObject();
            DLOG(info) << "Destroying Object " << object->GetObjectId() << " after " << delay_in_secs << " seconds.";

            simulation_service_->RemoveObject(object);

            kernel_->GetEventDispatcher()->Dispatch(
                make_shared<ValueEvent<shared_ptr<swganh::object::Object>>>("ObjectRemovedEvent", object));
        }
    }
}