// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

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
			_loadBuildings(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())), 
				real_event->scene_id, real_event->scene_label);

			statement->getMoreResults();
			_loadCells(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())), 
				real_event->scene_id, real_event->scene_label);
		
			statement->getMoreResults();
			_loadCloneLocations(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())), 
				real_event->scene_id, real_event->scene_label);
		
			statement->getMoreResults();
			_loadTerminals(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())),
				real_event->scene_id, real_event->scene_label);
		
			statement->getMoreResults();
			_loadElevatorData(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())), 
				real_event->scene_id, real_event->scene_label);
		
			statement->getMoreResults();
			_loadContainers(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())), 
				real_event->scene_id, real_event->scene_label);
		
			statement->getMoreResults();
			_loadTicketCollectors(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())),
				real_event->scene_id, real_event->scene_label);
		
			statement->getMoreResults();
			_loadNPCS(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())), 
				real_event->scene_id, real_event->scene_label);
		
			statement->getMoreResults();
			_loadShuttles(simulation_service, std::move(std::unique_ptr<sql::ResultSet>(statement->getResultSet())),
				real_event->scene_id, real_event->scene_label);

		} catch(std::exception& e) {
			LOG(warning) << e.what();
		}
		if (real_event->scene_id-1 == 0)
		{
			// Create a combat dummy
			auto simulation_service = kernel_->GetServiceManager()->GetService<SimulationServiceInterface>("SimulationService");
			auto combat_dummy = simulation_service->CreateObjectFromTemplate("object/mobile/shared_juvenile_canyon_krayt.iff", CREATURE_PERMISSION, false, false);
			auto creature_dummy = std::static_pointer_cast<Creature>(combat_dummy);
			creature_dummy->SetPosition(glm::vec3(-146, 28,-4702));
			creature_dummy->SetCustomName(L"Combat Dummy");
			creature_dummy->SetStatMax(HEALTH, 9999);
			creature_dummy->SetStatCurrent(HEALTH, 9999);
			creature_dummy->SetStatMax(ACTION, 9999);
			creature_dummy->SetStatCurrent(ACTION, 9999);
			creature_dummy->SetStatMax(MIND, 9999);
			creature_dummy->SetStatCurrent(MIND, 9999);
			creature_dummy->SetScale(0.5);
			
			creature_dummy->SetPvPStatus(PvPStatus_Attackable);
			simulation_service->AddObjectToScene(combat_dummy, "corellia");
		}
	});
}

void StaticService::Startup()
{
	auto database_manager = kernel_->GetDatabaseManager();
	auto conn = database_manager->getConnection("swganh_static");

	std::stringstream ss;
	ss << "CALL sp_GetStaticData();";

	auto statement = std::shared_ptr<sql::Statement>(conn->createStatement());
	statement->execute(ss.str());

	skill_mod_manager_.Start(statement);
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

void StaticService::_loadBuildings(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	simulation_service->PrepareToAccomodate(result->rowsCount());

	while (result->next())
	{
		//Load Building Row
		auto object = simulation_service->CreateObjectFromTemplate(result->getString(9), 
			STATIC_CONTAINER_PERMISSION, false, false, result->getInt64(1));
		
		if(object == nullptr)
			continue;

		object->SetOrientation(glm::quat(
			static_cast<float>(result->getDouble(5)),
			static_cast<float>(result->getDouble(2)),
			static_cast<float>(result->getDouble(3)),
			static_cast<float>(result->getDouble(4))));

		object->SetPosition(glm::vec3(result->getDouble(6), result->getDouble(7), result->getDouble(8)));
		object->SetStfName(result->getString(12), result->getString(13));
		object->SetSceneId(scene_id);
		object->SetInSnapshot(true);		
			
		//Put it into the scene
		simulation_service->AddObjectToScene(object, scene_name);
	}
}

void StaticService::_loadCells(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	while(result->next())
	{
		//Load Cells
		auto object = simulation_service->CreateObjectFromTemplate("object/cell/shared_cell.iff",
			WORLD_CELL_PERMISSION, false, false, result->getInt64(1));

		if(object == nullptr)
			continue;

		object->SetSceneId(scene_id);
		object->SetInSnapshot(true);
		
		auto parent = simulation_service->GetObjectById(result->getInt64(2));
		if(parent != nullptr)
		{
			parent->AddObject(nullptr, object);
		}

	}
}

void StaticService::_loadCloneLocations(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	while(result->next())
	{
		//TODO: Fill me in
	}
}

void StaticService::_loadTerminals(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	simulation_service->PrepareToAccomodate(result->rowsCount());

	while(result->next())
	{
		auto object = std::static_pointer_cast<Tangible>(simulation_service->CreateObjectFromTemplate(result->getString(11),
			DEFAULT_PERMISSION, false, false, result->getInt64(1)));

		if(object == nullptr)
			continue;

		object->SetOrientation(glm::quat(
			static_cast<float>(result->getDouble(6)),
			static_cast<float>(result->getDouble(3)),
			static_cast<float>(result->getDouble(4)),
			static_cast<float>(result->getDouble(5))));

		object->SetPosition(glm::vec3(result->getDouble(7),result->getDouble(8),result->getDouble(9)));

		object->SetStfName(result->getString(13), result->getString(12)); 

		std::string name = result->getString(16);
		object->SetCustomName(std::wstring(name.begin(), name.end()));

		if(object->GetObjectId() < 4294967297)
			object->SetInSnapshot(true);

		//Put it into the scene
		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, scene_name);
		}
		else
		{
			auto parent = simulation_service->GetObjectById(parent_id);
			if(parent != nullptr)
			{
				parent->AddObject(nullptr, object);
			}
		}
		// Check if it's a bank
		// TODO: Remove once prototypes are in..
		if (object->GetTemplate().compare("object/tangible/terminal/shared_terminal_bank.iff") == 0)
		{
			object->SetAttribute("radial_filename", L"radials.bank");
		}
	}
}

void StaticService::_loadElevatorData(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	while(result->next())
	{
		std::shared_ptr<ElevatorData> elevator_data = std::make_shared<ElevatorData>();

		uint64_t terminal_id = result->getUInt64(1);
		auto terminal = simulation_service->GetObjectById(terminal_id);
		if(terminal == nullptr)
			continue;
		
		terminal->SetAttribute<std::wstring>("radial_filename", L"radials.elevator");

		elevator_data->dst_cell = result->getUInt64(2);
		elevator_data->dst_orientation = glm::quat(
			static_cast<float>(result->getDouble(6)),
			static_cast<float>(result->getDouble(3)),
			static_cast<float>(result->getDouble(4)),
			static_cast<float>(result->getDouble(5)));
		elevator_data->dst_position = glm::vec3(result->getDouble(7),result->getDouble(8),result->getDouble(9));
		elevator_data->effect_id = result->getUInt(10);
		elevator_data->going_down = result->getUInt(11) != 0;

		if(elevator_data->going_down)
			terminal->SetAttribute<int32_t>("elevator_can_go_down", 0);
		else
			terminal->SetAttribute<int32_t>("elevator_can_go_up", 0);

		auto find_itr = elevator_lookup_.find(terminal_id);
		if(find_itr == elevator_lookup_.end())
			find_itr = elevator_lookup_.insert(std::make_pair(terminal_id, std::vector<std::shared_ptr<ElevatorData>>())).first;

		find_itr->second.push_back(elevator_data);
	}
}

void StaticService::_loadContainers(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	while(result->next())
	{
	}
}

void StaticService::_loadTicketCollectors(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	simulation_service->PrepareToAccomodate(result->rowsCount());

	while(result->next())
	{
		auto object = std::static_pointer_cast<Tangible>(simulation_service->CreateObjectFromTemplate(result->getString(3),
			DEFAULT_PERMISSION, false, false, result->getInt64(1)));

		if(object == nullptr)
			continue;

		object->SetOrientation(glm::quat(
			static_cast<float>(result->getDouble(7)),
			static_cast<float>(result->getDouble(4)),
			static_cast<float>(result->getDouble(5)),
			static_cast<float>(result->getDouble(6))));

		object->SetPosition(glm::vec3(result->getDouble(8),result->getDouble(9),result->getDouble(10)));

		object->SetStfName(result->getString(13), result->getString(12));

		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, scene_name);
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

void StaticService::_loadNPCS(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	simulation_service->PrepareToAccomodate(result->rowsCount());

	while(result->next())
	{
		//Load NPCS
		auto object = std::static_pointer_cast<Creature>(simulation_service->CreateObjectFromTemplate(result->getString(15),
			CREATURE_PERMISSION, false, false, result->getUInt64(1)));

		if(object == nullptr)
			continue;

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
			static_cast<float>(result->getDouble(11)),
			static_cast<float>(result->getDouble(8)),
			static_cast<float>(result->getDouble(9)),
			static_cast<float>(result->getDouble(10))));

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

		object->SetSceneId(scene_id);
		object->SetInSnapshot(false);
		
		//Put it into the scene
		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, scene_name);
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

void StaticService::_loadShuttles(SimulationServiceInterface* simulation_service, std::unique_ptr<sql::ResultSet> result,
	uint32_t scene_id, std::string scene_name)
{
	simulation_service->PrepareToAccomodate(result->rowsCount());

	while(result->next())
	{
		auto object = std::static_pointer_cast<Creature>(simulation_service->CreateObjectFromTemplate(result->getString(12),
			DEFAULT_PERMISSION, false, false, result->getInt64(1)));

		if(object == nullptr)
			continue;

		object->SetOrientation(glm::quat(
			static_cast<float>(result->getDouble(8)),
			static_cast<float>(result->getDouble(5)),
			static_cast<float>(result->getDouble(6)),
			static_cast<float>(result->getDouble(7))));

		object->SetPosition(glm::vec3(result->getDouble(9),result->getDouble(10),result->getDouble(11)));

		object->SetStfName(result->getString(14), result->getString(13));

		uint64_t awayTime = result->getUInt64(15);
		uint64_t inPortTime = result->getUInt64(16);
		uint64_t collectorId = result->getUInt64(17);

		object->SetPvPStatus(PvPStatus_None);
		object->SetOptionsMask(OPTION_NO_HAM);
		
		uint64_t parent_id = result->getUInt64(2);
		if(parent_id == 0)
		{
			simulation_service->AddObjectToScene(object, scene_name);
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

std::vector<std::shared_ptr<ElevatorData>> StaticService::GetElevatorDataForObject(uint64_t terminal_id)
{
	auto find_itr = elevator_lookup_.find(terminal_id);
	if(find_itr != elevator_lookup_.end())
	{
		return find_itr->second;
	}
	return std::vector<std::shared_ptr<ElevatorData>>();
}

std::pair<uint32_t, uint32_t> StaticService::GetSkillMod(const std::shared_ptr<swganh::object::Creature>& creature, const std::string& skill_mod_name)
{
	return skill_mod_manager_.GetSkillMod(creature, skill_mod_name);
}
		
std::map<std::string, std::pair<uint32_t, uint32_t>> StaticService::GetSkillMods(const std::shared_ptr<swganh::object::Creature>& creature)
{
	return skill_mod_manager_.GetSkillMods(creature);
}