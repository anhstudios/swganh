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
#include "swganh_core/object/tangible/tangible.h"

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
		ss << "CALL sp_GetStaticObjects(0," << real_event->scene_id-1 << ");";

		auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
		statement->execute(ss.str());

		std::unique_ptr<sql::ResultSet> result;

		LOG(warning) << "Loading static data for: " << real_event->scene_label;
		_loadBuildings(simulation_service, statement->getResultSet());
		_loadCells(simulation_service, statement->getResultSet());
		_loadCloneLocations(simulation_service, statement->getResultSet());
		_loadTerminals(simulation_service, statement->getResultSet());
		_loadElevatorData(simulation_service, statement->getResultSet());
		_loadContainers(simulation_serivce, statement->getResultSet());
		_loadTicketCollectors(simulation_service, statement->getResultSet());
		_loadNPCS(simulation_service, statement->getResultSet());
		_loadShuttles(simulation_serivce, statement->getResultSet());

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

void StaticService::_loadBuildings(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while (result->next())
	{
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
}

void StaticService::_loadCells(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while(result->next())
	{
		//Load Cells
		auto object = simulation_service->CreateObjectFromTemplate("object/cell/shared_cell.iff",
			WORLD_CELL_PERMISSION, false, false, result->getInt64(1));

		object->SetSceneId(real_event->scene_id);
		object->SetInSnapshot(true);
		object->SetDatabasePersisted(false);

		auto parent = simulation_service->GetObjectById(result->getInt64(2));
		if(parent != nullptr)
		{
			parent->AddObject(nullptr, object);
		}

	}
}

void StaticService::_loadCloneLocations(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while(result->next())
	{
		//TODO: Fill me in
	}
}

void StaticService::_loadTerminals(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while(result->next())
	{
		auto object = std::static_pointer_cast<Tangible>(simulation_service->CreateObjectFromTemplate(result->getString(11),
			DEFAULT_PERMISSION, false, false, result->getInt64(1)));

		object->SetOrientation(glm::quat(
			static_cast<float>(result->getDouble(3)),
			static_cast<float>(result->getDouble(4)),
			static_cast<float>(result->getDouble(5)),
			static_cast<float>(result->getDouble(6))));

		object->SetOrientation(glm::rotate(object->GetOrientation(), 180, glm::vec3(1.0f, 0.0f, 0.0f)));
		object->SetOrientation(glm::rotate(object->GetOrientation(), 180, glm::vec3(0.0f, 1.0f, 0.0f)));

		object->SetPosition(glm::vec3(result->getDouble(7),result->getDouble(8),result->getDouble(9)));

		object->SetStfName(result->getString(13), result->getString(12)); 

		std::string name = result->getString(16);
		object->SetCustomName(std::wstring(name.begin(), name.end()));

		//Put it into the scene
		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, real_event->scene_label);
		}
		else
		{
			auto parent = simulation_service->GetObjectById(parent_id);
			if(parent != nullptr)
			{
				parent->AddObject(nullptr, object);
			}
		}
	}
}

void StaticService::_loadElevatorData(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while(result->next())
	{
		//TODO: Fill me in
	}
}

void StaticService::_loadTicketCollectors(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while(result->next())
	{
		auto object = std::static_pointer_cast<Tangible>(simulation_service->CreateObjectFromTemplate(result->getString(3),
			DEFAULT_PERMISSION, false, false, result->getInt64(1)));

		object->SetOrientation(glm::quat(
			static_cast<float>(result->getDouble(4)),
			static_cast<float>(result->getDouble(5)),
			static_cast<float>(result->getDouble(6)),
			static_cast<float>(result->getDouble(7))));

		object->SetOrientation(glm::rotate(object->GetOrientation(), 180, glm::vec3(1.0f, 0.0f, 0.0f)));
		object->SetOrientation(glm::rotate(object->GetOrientation(), 180, glm::vec3(0.0f, 1.0f, 0.0f)));

		object->SetPosition(glm::vec3(result->getDouble(8),result->getDouble(9),result->getDouble(10)));

		object->SetStfName(result->getString(13), result->getString(12));

		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, real_event->scene_label);
		}
		else
		{
			auto parent = simulation_service->GetObjectById(parent_id);
			if(parent != nullptr)
			{
				parent->AddObject(nullptr, object);
			}
		}
	}
}

void StaticService::_loadNPCS(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while(result->next())
	{
		//Load NPCS
		auto object = std::static_pointer_cast<Creature>(simulation_service->CreateObjectFromTemplate(result->getString(15),
			CREATURE_PERMISSION, false, false, result->getUInt64(1)));

		std::string firstname = result->getString(3).asStdString(), lastname = result->getString(4).asStdString();

		if(firstname.size() != 0) {
			std::string name(firstname + " " + lastname);
			object->SetCustomName(std::wstring(name.begin(), name.end()));
		}

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

		object->SetPvPStatus(PvPStatus_None);

		switch(result->getUInt(20))
		{
		case TRAINER:
			object->SetOptionsMask(OPTION_TRAINER | OPTION_NO_HAM);
			break;
		case FILLER:
			object->SetOptionsMask(OPTION_NO_HAM);
			break;
		case QUEST_GIVER:
			object->SetOptionsMask(OPTION_QUEST_NPC | OPTION_NO_HAM);
			break;
		case JUNK_DEALER:
		case CHASSIS_DEALER:
			object->SetOptionsMask(OPTION_VENDOR | OPTION_NO_HAM);
			break;
		};

		object->SetSceneId(real_event->scene_id);
		object->SetInSnapshot(false);
		object->SetDatabasePersisted(false);

		//Put it into the scene
		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, real_event->scene_label);
		}
		else
		{
			auto parent = simulation_service->GetObjectById(parent_id);
			if(parent != nullptr)
			{
				parent->AddObject(nullptr, object);
			}
		}
	}
}

void StaticService::_loadShuttles(std::shared_ptr<SimulationServiceInterface> simulation_service, std::unique_ptr<sql::ResultSet>& result)
{
	while(result->next())
	{
		auto object = std::static_pointer_cast<Tangible>(simulation_service->CreateObjectFromTemplate(result->getString(12),
			DEFAULT_PERMISSION, false, false, result->getInt64(1)));

		object->SetOrientation(glm::quat(
			static_cast<float>(result->getDouble(5)),
			static_cast<float>(result->getDouble(6)),
			static_cast<float>(result->getDouble(7)),
			static_cast<float>(result->getDouble(8))));

		object->SetPosition(glm::vec3(result->getDouble(9),result->getDouble(10),result->getDouble(11)));

		object->SetStfName(result->getString(14), result->getString(13));

		uint64_t awayTime = result->getUInt64(15);
		uint64_t inPortTime = result->getUInt64(16);
		uint64_t collectorId = result->getUInt64(17);

		object->SetOptionsMask(OPTION_NO_HAM);

		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, real_event->scene_label);
		}
		else
		{
			auto parent = simulation_service->GetObjectById(parent_id);
			if(parent != nullptr)
			{
				parent->AddObject(nullptr, object);
			}
		}
	}
}