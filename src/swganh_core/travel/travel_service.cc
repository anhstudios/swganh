// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <swganh_core/travel/travel_service.h>

#include <swganh/service/service_manager.h>

#include <swganh_core/messages/enter_ticket_purchase_mode_message.h>
#include <swganh_core/object/object.h>
#include <swganh_core/simulation/simulation_service.h>

using namespace swganh::travel;
using namespace swganh::object;
using namespace swganh::service;
using namespace swganh::simulation;

TravelService::TravelService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
}

TravelService::~TravelService(void)
{
}

void TravelService::Startup()
{
	simulation_ = kernel_->GetServiceManager()->GetService<SimulationService>("SimulationService");
}

swganh::service::ServiceDescription TravelService::GetServiceDescription()
{
	return swganh::service::ServiceDescription(
		"TravelService", // namve
		"travel", // type
		"0.1", // version
		"127.0.0.1", // address
		0, // tcp port
		0, // udp port
		0); // status
}

void TravelService::BeginTicketTransaction(std::shared_ptr<Object> object)
{
	if(!object->HasController())
		return;

	swganh::messages::EnterTicketPurchaseModeMessage enter_ticket;
	enter_ticket.planet_name = simulation_->SceneNameById(object->GetSceneId());
	enter_ticket.city_name = "cornet";
	object->GetController()->Notify(&enter_ticket);
}