// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <swganh_core/map/map_service.h>

#include <swganh/app/swganh_kernel.h>
#include <swganh/service/service_manager.h>
#include <swganh/database/database_manager.h>
#include <swganh/logger.h>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <swganh_core/messages/get_map_locations_request_message.h>


using namespace swganh::map;

using swganh::service::ServiceDescription;

MapService::MapService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{
}

MapService::~MapService()
{
}

ServiceDescription MapService::GetServiceDescription()
{
	return ServiceDescription(
		"MapService",
		"map",
		"0.1",
		"127.0.0.1",
		0,
		0,
		0
		);
}

void MapService::Startup()
{
	auto conn = kernel_->GetDatabaseManager()->getConnection("swganh_static");
}