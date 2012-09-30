#include "static_service.h"

#include "swganh/event_dispatcher.h"

#include <memory>

#include "swganh/logger.h"

#include "swganh/database/database_manager.h"
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include "swganh/service/service_manager.h"
#include "swganh/object/permissions/permission_type.h"

#include "swganh_core/simulation/simulation_service.h"
#include "swganh_core/simulation/scene_events.h"
#include "swganh_core/object/object.h"
#include "swganh_core/object/creature/creature.h"

using namespace swganh::object;
using namespace swganh::statics;
using namespace swganh::service;
using namespace swganh::app;
using namespace swganh::simulation;

enum PERSISTENT_NPC_TYPE
{
	TRAINER = 1,
	FILLER,
	QUEST_GIVER,
	JUNK_DEALER,
	CHASSIS_DEALER,
	RECRUITER
};

StaticService::StaticService(SwganhKernel* kernel)
{
	//Static Objects
	kernel->GetEventDispatcher()->Subscribe("SceneManager:NewScene", [&] (const std::shared_ptr<swganh::EventInterface>& newEvent)
	{
		auto real_event = std::static_pointer_cast<swganh::simulation::NewSceneEvent>(newEvent);

		auto database_manager = kernel->GetDatabaseManager();
		auto conn = database_manager->getConnection("swganh_static");

		auto simulation_service = kernel->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");


		auto statement = conn->prepareStatement("sp_GetStaticObjects(?, ?)");
		statement->setBigInt(0, 0);
		statement->setInt(1, real_event->scene_id);

		LOG(info) << "Loading Buildings for Planet: " << real_event->scene_label;

		auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());        
		while (result->next())
		{
			//Load Building Row
			auto object = simulation_service->CreateObjectFromTemplate(result->getString(8), 
				STATIC_CONTAINER_PERMISSION, false, false, result->getInt64(0));
			
			object->SetOrientation(glm::quat(
				static_cast<float>(result->getDouble(1)),
				static_cast<float>(result->getDouble(2)),
				static_cast<float>(result->getDouble(3)),
				static_cast<float>(result->getDouble(4))));

			object->SetPosition(glm::vec3(result->getDouble(5), result->getDouble(6), result->getDouble(7)));
			object->SetStfName(result->getString(11), result->getString(12));
			object->SetSceneId(real_event->scene_id);
			object->SetInSnapshot(true);
			object->SetDatabasePersisted(false);
			
			//Put it into the scene
			simulation_service->TransferObjectToScene(object, real_event->scene_label);
		}
		
		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//Load Cells
			auto object = simulation_service->CreateObjectFromTemplate("object/cell/shared_cell.iff",
				WORLD_CELL_PERMISSION, false, false, result->getInt64(0));

			object->SetSceneId(real_event->scene_id);
			object->SetInSnapshot(false);
			object->SetDatabasePersisted(false);

			auto parent = simulation_service->GetObjectById(result->getInt64(1));
			if(parent != nullptr)
			{
				parent->AddObject(nullptr, object);
			}

		}

		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO: Load Spawn Points
		}

		LOG(info) << "Loading Terminals for Planet: " << real_event->scene_label;

		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO: Load Terminals

		}

		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO:: Load Elevator Data

		}

		LOG(info) << "Loading Containers for Planet: " << real_event->scene_label;
		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO:: Load Containers

		}
		
		LOG(info) << "Loading Ticket Collectors for Planet: " << real_event->scene_label;
		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO:: Load Ticket Collectors

		}

		LOG(info) << "Loading NPCs for Planet: " << real_event->scene_label;
		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//Load NPCS
			auto object = std::static_pointer_cast<Creature>(simulation_service->CreateObjectFromTemplate(result->getString(14),
				CREATURE_PERMISSION, false, false, result->getInt64(0)));

			std::string name(result->getString(2).asStdString() + " " + result->getString(3).asStdString());
			object->SetCustomName(std::wstring(name.begin(), name.end()));

			object->SetPosture((Posture)result->getUInt(4));
			object->SetStateBitmask(result->getUInt(5));
			object->SetCombatLevel(result->getUInt(6));

			object->SetOrientation(glm::quat(
				static_cast<float>(result->getDouble(7)),
				static_cast<float>(result->getDouble(8)),
				static_cast<float>(result->getDouble(9)),
				static_cast<float>(result->getDouble(10))));

			object->SetPosition(glm::vec3(result->getDouble(11),result->getDouble(12),result->getDouble(13)));
			object->SetMoodId(result->getUInt(18));
			object->SetScale(static_cast<float>(result->getDouble(20)));

			switch(result->getUInt(19))
			{
			case TRAINER:
			case FILLER:
			case QUEST_GIVER:
			case JUNK_DEALER:
			case CHASSIS_DEALER:
			case RECRUITER:
				break;
			};

			object->SetSceneId(real_event->scene_id);
			object->SetInSnapshot(false);
			object->SetDatabasePersisted(false);

			//Put it into the scene
			simulation_service->TransferObjectToScene(object, real_event->scene_label);
		}

		LOG(info) << "Loading Shuttles for Planet: " << real_event->scene_label;
		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO:: Load Shuttles

		}

		LOG(info) << "Loading Items for Planet: " << real_event->scene_label;
		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO:: Load Items

		}

		LOG(info) << "Loading Resource Containers for Planet: " << real_event->scene_label;
		statement->getMoreResults();
		result = std::unique_ptr<sql::ResultSet>(statement->getResultSet());        
		while(result->next())
		{
			//TODO:: Load Resource Containers

		}
	});
}

StaticService::~StaticService()
{
}

ServiceDescription StaticService::GetServiceDescription()
{
	ServiceDescription service_description(
        "StaticService",
        "static",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}