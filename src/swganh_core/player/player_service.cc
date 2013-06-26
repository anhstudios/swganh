// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "player_service.h"

#include "swganh/logger.h"
#include "swganh/event_dispatcher.h"
#include "swganh/service/service_description.h"
#include "swganh/service/service_manager.h"
#include "swganh_core/connection/connection_client_interface.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/object_controller.h"
#include "swganh_core/messages/system_message.h"
#include "swganh_core/messages/out_of_band.h"
#include "swganh_core/messages/opened_container_message.h"
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/equipment/equipment_service.h"

#include "swganh_core/messages/controllers/add_buff.h"
#include "swganh_core/combat/buff_interface.h"

#include "swganh/database/database_manager.h"
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

using namespace std;
using namespace swganh;
using namespace swganh::service;
using namespace swganh::player;
using namespace swganh::object;
using namespace swganh::messages;

PlayerService::PlayerService(swganh::app::SwganhKernel* kernel)
	: kernel_(kernel)
{    
    SetServiceDescription(ServiceDescription(
        "PlayerService",
        "player",
        "0.1",
        "127.0.0.1", 
        0, 
        0, 
        0));
}

PlayerService::~PlayerService()
{}

void PlayerService::Initialize()
{
	simulation_service_ = kernel_->GetServiceManager()->
		GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");

	equipment_service_ = kernel_->GetServiceManager()->
		GetService<swganh::equipment::EquipmentService>("EquipmentService");
}

void PlayerService::Startup()
{
	kernel_->GetEventDispatcher()->Subscribe(
		"ObjectReadyEvent",
		[this] (shared_ptr<EventInterface> incoming_event)
	{
		auto creature = static_pointer_cast<ValueEvent<shared_ptr<Creature>>>(incoming_event)->Get();

		//Reload buffs from last login
		auto controller = creature->GetController();
		auto object_id = creature->GetObjectId();
		
		//Resend buffs we current have left over
		creature->ViewBuffs([&, this] (std::pair<boost::posix_time::ptime, std::shared_ptr<swganh::combat::BuffInterface>> entry) {
			uint32_t duration = (entry.first - boost::posix_time::second_clock::local_time()).total_seconds();

			swganh::messages::controllers::AddBuffMessage msg;
			msg.buff = entry.second->GetName();
			msg.duration = static_cast<float>(duration);
			controller->Notify(&msg);
		});

		//Re-Add buffs from the db we still need.
		auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
		auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        
        stringstream ss;
        ss << "SELECT b.name, b.duration FROM buffs b WHERE b.id=" << object_id << ";" ;
        statement->execute(ss.str());
		
		unique_ptr<sql::ResultSet> result(statement->getResultSet());

		while(result->next())
		{
			creature->AddBuff(result->getString(1), result->getUInt(2));
		}

		OnPlayerEnter(std::static_pointer_cast<Player>(equipment_service_->GetEquippedObject(creature, "ghost")));
	});

	kernel_->GetEventDispatcher()->Subscribe(
		"Connection::ControllerConnectionClosed",
		[this] (shared_ptr<EventInterface> incoming_event)
	{
		auto object_id = static_pointer_cast<ValueEvent<uint64_t>>(incoming_event)->Get();
        auto player = simulation_service_->GetObjectById<Player>(object_id + 1);

        if(player) OnPlayerExit(player);
	});
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
        auto deadline_timer = std::make_shared<boost::asio::deadline_timer>(kernel_->GetCpuThreadPool(), boost::posix_time::seconds(30));
		auto parent = std::static_pointer_cast<swganh::object::Object>(player->GetContainer());

		auto object_controller = std::static_pointer_cast<swganh::object::ObjectController>(parent->GetController());
		if(object_controller != nullptr)
		{
			deadline_timer->async_wait(boost::bind(&PlayerService::RemoveClientTimerHandler_, this, boost::asio::placeholders::error, deadline_timer, 30, object_controller));
		}
    }
}

void PlayerService::SendTip(
	const shared_ptr<Creature>& from, 
	const shared_ptr<Creature>& to, 
	uint32_t amount, bool bank)
{
	// Check to see if we even have the proper amount
	if (amount < 1 || amount > 1000000)
	{
		SystemMessage::Send(from, L"Invalid tip amount, set amount between 1 and 1,000,000");
		return;
	}

	if (bank)
	{
		int32_t total_amount = (int32_t)(amount * 1.05); // 5% tip charge
		int32_t from_amount = from->GetBankCredits();
		if ( from_amount - total_amount > 0)
		{
			from->SetBankCredits( from_amount - total_amount );
			int32_t to_amount = to->GetBankCredits();
			to->SetBankCredits( to_amount + amount );
			SystemMessage::Send(from, OutOfBand("base_player", "prose_wire_pass_self",0,0,to->GetObjectId(),amount), false, false);
			SystemMessage::Send(to, OutOfBand("base_player", "prose_wire_pass_target",0,0,from->GetObjectId(),amount), false, false);
		}
		else
		{
			SystemMessage::Send(from, OutOfBand("base_player", "prose_tip_nsf_bank",0,to->GetObjectId(),0,total_amount), false, false);			
		}
	}
	else
	{
		int32_t total_amount = (int32_t)(amount * 1.05); // 5% tip charge
		int32_t from_amount = from->GetCashCredits();
		if ( from_amount - total_amount > 0)
		{
			from->SetCashCredits( from_amount - total_amount );
			int32_t to_amount = to->GetBankCredits();
			to->SetCashCredits( to_amount + amount );
			SystemMessage::Send(from, OutOfBand("base_player", "prose_tip_pass_self",0,to->GetObjectId(),0,amount), false, false);
			SystemMessage::Send(to, OutOfBand("base_player", "prose_tip_pass_target",0,from->GetObjectId(),0,amount), false, false);
		}
		else
		{
			SystemMessage::Send(from, OutOfBand("base_player", "prose_tip_nsf_cash",0,to->GetObjectId(),0,total_amount), false, false);
		}
	}
}

void PlayerService::OpenContainer(const std::shared_ptr<swganh::object::Creature>& owner, std::shared_ptr<swganh::object::Object> object)
{
	// Send Open Container
	OpenedContainerMessage opened_container;
	opened_container.container_object_id = object->GetObjectId();
	owner->NotifyObservers(&opened_container);
}

bool PlayerService::HasCalledMount(std::shared_ptr<swganh::object::Creature> owner)
{
	auto equipment = kernel_->GetServiceManager()->GetService<swganh::equipment::EquipmentServiceInterface>("EquipmentService");
	auto datapad = equipment->GetEquippedObject(owner, "datapad");
	bool result = false;
	if(datapad)
	{
		datapad->ViewObjects(nullptr, 0, true, [&] (std::shared_ptr<Object> object) {
			if(!result && object->HasAttribute("is_mount") && !object->HasContainedObjects())
			{
				result = true;
			}
		});
	}
	return result;
}

void PlayerService::StoreAllCalledMounts(std::shared_ptr<swganh::object::Creature> owner)
{
	auto simulation = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");
	auto equipment = kernel_->GetServiceManager()->GetService<swganh::equipment::EquipmentServiceInterface>("EquipmentService");
	auto datapad = equipment->GetEquippedObject(owner, "datapad");
	if(datapad)
	{
		std::list<std::shared_ptr<Object>> objects = datapad->GetObjects(nullptr, 1, true);
		for(auto& object : objects) 
        {
			if(object->HasAttribute("is_mount") && !object->HasContainedObjects())
			{
				if(auto mobile = simulation->GetObjectById((uint64_t)object->GetAttribute<int64_t>("mobile_id")))
				{
                    if (owner->GetContainer()->GetObjectId() == mobile->GetObjectId())
                    {
                        mobile->TransferObject(owner, owner, mobile->GetContainer(), mobile->GetPosition());
                    }

					mobile->GetContainer()->TransferObject(owner, mobile, object, glm::vec3(0, 0, 0));
				}
			}
		}
	}
}
	
void PlayerService::StoreAllCalledObjects(std::shared_ptr<swganh::object::Creature> owner)
{
	auto simulation = kernel_->GetServiceManager()->GetService<swganh::simulation::SimulationServiceInterface>("SimulationService");
	auto equipment = kernel_->GetServiceManager()->GetService<swganh::equipment::EquipmentServiceInterface>("EquipmentService");
	auto datapad = equipment->GetEquippedObject(owner, "datapad");
	if(datapad)
	{
		datapad->ViewObjects(nullptr, 0, true, [&] (std::shared_ptr<Object> object)
        {
			if(object->HasAttribute("mobile_id") && !object->HasContainedObjects())
			{
				if(auto mobile = simulation->GetObjectById((uint64_t)object->GetAttribute<int64_t>("mobile_id")))
				{
                    if (owner->GetContainer()->GetObjectId() == mobile->GetObjectId())
                    {
                        mobile->TransferObject(owner, owner, mobile->GetContainer(), mobile->GetPosition());
                    }

					mobile->GetContainer()->TransferObject(owner, mobile, object, glm::vec3(0, 0, 0));
				}
			}
		});
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

			//Persist buffs
			auto conn = kernel_->GetDatabaseManager()->getConnection("galaxy");
			auto object_id = object->GetObjectId();
			auto creature = std::static_pointer_cast<Creature>(object);
			creature->ViewBuffs([this,&conn,&object_id] (std::pair<boost::posix_time::ptime, std::shared_ptr<swganh::combat::BuffInterface>> entry) {
				std::shared_ptr<sql::PreparedStatement> statement(conn->prepareStatement("INSERT INTO buffs VALUES (?,?,?)"));

				uint32_t duration = (entry.first - boost::posix_time::second_clock::local_time()).total_seconds();

				statement->setUInt64(1, object_id);
				statement->setString(2, entry.second->filename);
				statement->setInt(3, duration);

				statement->execute();
			});

            StoreAllCalledMounts(creature);
			creature->CleanUpBuffs();
            
            simulation_service_->RemoveObject(object);

            kernel_->GetEventDispatcher()->Dispatch(
                make_shared<ValueEvent<shared_ptr<swganh::object::Object>>>("ObjectRemovedEvent", object));
        }
    }
}
