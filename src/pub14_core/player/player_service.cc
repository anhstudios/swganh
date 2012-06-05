// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "player_service.h"

#include "anh/logger.h"
#include "anh/event_dispatcher.h"
#include "anh/service/service_description.h"
#include "anh/service/service_manager.h"
#include "swganh/connection/connection_client.h"
#include "swganh/object/player/player.h"
#include "swganh/object/object_controller.h"
#include "swganh/simulation/simulation_service_interface.h"

using namespace std;
using namespace anh;
using namespace anh::service;
using namespace swganh_core::player;
using namespace swganh::object::player;


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

void PlayerService::CleanupPlayerState(shared_ptr<swganh::object::player::Player> player)
{

}

void PlayerService::OnPlayerEnter(shared_ptr<swganh::object::player::Player> player)
{
	player->ClearStatusFlags();
}

void PlayerService::OnPlayerExit(shared_ptr<swganh::object::player::Player> player)
{
	player->ClearStatusFlags();
	player->AddStatusFlag(swganh::object::player::LD);
	// set a timer to 5 minutes to destroy the object, unless logged back in.
    auto deadline_timer = std::make_shared<boost::asio::deadline_timer>(kernel_->GetIoService(), boost::posix_time::seconds(30));
    deadline_timer->async_wait(boost::bind(&PlayerService::RemoveClientTimerHandler_, this, boost::asio::placeholders::error, deadline_timer, 10, player->GetContainer()->GetController()));
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
            LOG(warning) << "Destroying Object " << object->GetObjectId() << " after " << delay_in_secs << " seconds.";

            simulation_service_->RemoveObject(object);

            kernel_->GetEventDispatcher()->Dispatch(
                make_shared<ValueEvent<shared_ptr<swganh::object::Object>>>("ObjectRemovedEvent", object));
        }
    }
}