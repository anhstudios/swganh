#include "static_service.h"

#include "swganh/event_dispatcher.h"

#include <memory>
#include <exception>

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
	: kernel_(kernel)
{
	//Static Objects
	kernel_->GetEventDispatcher()->Subscribe("SceneManager:NewScene", [&] (const std::shared_ptr<swganh::EventInterface>& newEvent)
	{
		auto real_event = std::static_pointer_cast<swganh::simulation::NewSceneEvent>(newEvent);

		auto database_manager = kernel_->GetDatabaseManager();
		auto conn = database_manager->getConnection("swganh_static");

		auto simulation_service = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");

		try {

		std::stringstream ss;

		ss << "CALL sp_GetStaticObjects(0," << real_event->scene_id << ");";

		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		statement->execute(ss.str());

		std::unique_ptr<sql::ResultSet> result;

		LOG(warning) << "Loading Buildings for Planet: " << real_event->scene_label;
		
		uint32_t tats = 0;

		result.reset(statement->getResultSet());
		while (result->next())
		{
			tats += 1;
			//Load Building Row
			auto object = simulation_service->CreateObjectFromTemplate(result->getString(9), 
				STATIC_CONTAINER_PERMISSION, false, false, result->getInt64(1));
			
			object->SetOrientation(glm::quat(
				static_cast<float>(result->getDouble(2)),
				static_cast<float>(result->getDouble(3)),
				static_cast<float>(result->getDouble(4)),
				static_cast<float>(result->getDouble(5))));

			object->SetPosition(glm::vec3(result->getDouble(6), result->getDouble(7), result->getDouble(8)));
			object->SetStfName(result->getString(12), result->getString(13));
			object->SetSceneId(real_event->scene_id);
			object->SetInSnapshot(true);
			object->SetDatabasePersisted(false);
			
			//Put it into the scene
			simulation_service->AddObjectToScene(object, real_event->scene_label);
		}

		LOG(warning) << tats;
		/*
		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());      
			while(result->next())
			{
				//Load Cells
				auto object = simulation_service->CreateObjectFromTemplate("object/cell/shared_cell.iff",
					WORLD_CELL_PERMISSION, false, false, result->getInt64(1));

				object->SetSceneId(real_event->scene_id);
				object->SetInSnapshot(true);
				object->SetDatabasePersisted(false);

				auto parent = simulation_service->GetObjectById(result->getInt64(1));
				if(parent != nullptr)
				{
					parent->AddObject(nullptr, object);
				}

			}
		}

		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());           
			while(result->next())
			{
				//TODO: Load Spawn Points
			}
		}

		LOG(warning) << "Loading Terminals for Planet: " << real_event->scene_label;

		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());        
			while(result->next())
			{
				//TODO: Load Terminals

			}
		}

		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());        
			while(result->next())
			{
				//TODO:: Load Elevator Data

			}
		}

		LOG(warning) << "Loading Containers for Planet: " << real_event->scene_label;
		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());        
			while(result->next())
			{
				//TODO:: Load Containers

			}
		}
		
		LOG(warning) << "Loading Ticket Collectors for Planet: " << real_event->scene_label;
		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());        
			while(result->next())
			{
				//TODO:: Load Ticket Collectors

			}
		}*/

		LOG(warning) << "Loading NPCs for Planet: " << real_event->scene_label;
		uint32_t tits = 0;
		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());        
			while(result->next())
			{
				//Load NPCS
				tits += 1;

				auto object = std::static_pointer_cast<Creature>(simulation_service->CreateObjectFromTemplate(result->getString(15),
					CREATURE_PERMISSION, false, false, result->getUInt64(1)));

				std::string firstname = result->getString(3).asStdString(), lastname = result->getString(4).asStdString();
				std::string name(firstname + " " + lastname);
				object->SetCustomName(std::wstring(name.begin(), name.end()));

				object->SetStfName(result->getString(17), result->getString(16));

				object->SetPosture((Posture)result->getUInt(5));
				object->SetStateBitmask(result->getUInt(6));
				object->SetCombatLevel(result->getUInt(7));

				object->SetOrientation(glm::quat(
					static_cast<float>(result->getDouble(8)),
					static_cast<float>(result->getDouble(9)),
					static_cast<float>(result->getDouble(10)),
					static_cast<float>(result->getDouble(11))));

				object->SetPosition(glm::vec3(result->getDouble(12),result->getDouble(13),result->getDouble(14)));
				object->SetMoodId(result->getUInt(19));
				object->SetScale(static_cast<float>(result->getDouble(21)));

				switch(result->getUInt(20))
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
				simulation_service->AddObjectToScene(object, real_event->scene_label);
			}
		}

		LOG(warning) << tits;

		LOG(warning) << "Loading Shuttles for Planet: " << real_event->scene_label;
		if(statement->getMoreResults())
		{
			result.reset(statement->getResultSet());        
			while(result->next())
			{
				//TODO:: Load Shuttles

			}
		}

		} catch(std::exception& e) {
			LOG(warning) << e.what();
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