// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "purchase_ticket_command.h"

#ifdef WIN32
#include <regex>
#else
#include <boost/regex.hpp>
#endif

#include <swganh/app/swganh_kernel.h>
#include <swganh/service/service_manager.h>

#include <swganh_core/travel/travel_service.h>

using swganh::app::SwganhKernel;
using swganh::travel::PurchaseTicketCommand;
using swganh::command::CommandProperties;

using namespace swganh::service;
using namespace swganh::app;

#ifdef WIN32
using std::wregex;
using std::wsmatch;
using std::regex_match;
#else
using boost::wregex;
using boost::wsmatch;
using boost::regex_match;
#endif

void PurchaseTicketCommand::Run()
{
	travel_ = GetKernel()->GetServiceManager()->GetService<TravelService>("TravelService");

	const wregex p(L"(.*) (.*) (.*) (.*) (\\d+)");
	wsmatch m;

	if(regex_match(GetCommandString(), m, p)) {
		auto source_scene = m[1].str();
		auto source_location = m[2].str();
		auto target_scene = m[3].str();
		auto target_location = m[4].str();

		travel_->PurchaseTicket(GetActor(), 
			std::string(source_scene.begin(), source_scene.end()), 
			std::string(source_location.begin(), source_location.end()),
			std::string(target_scene.begin(), target_scene.end()),
			std::string(target_location.begin(), target_location.end()),
			0,
			0,
			std::stoi(m[5].str()) ? true : false);
	}
}
