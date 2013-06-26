// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "player_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"
#include "swganh/plugin/plugin_manager.h"

#include "swganh/crc.h"
#include "swganh/database/database_manager.h"
#include "swganh_core/object/player/player.h"
#include "swganh_core/object/object_manager.h"
#include "player_events.h"

#include "swganh_core/object/exception.h"
#include "swganh_core/simulation/simulation_service_interface.h"
#include "swganh_core/character/character_provider_interface.h"

using namespace std;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;
using namespace swganh::character;

PlayerFactory::PlayerFactory(swganh::app::SwganhKernel* kernel)
    : IntangibleFactory(kernel)
{
	character_provider_ = kernel_->GetPluginManager()->CreateObject<CharacterProviderInterface>("Character::CharacterProvider");
}

void PlayerFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    IntangibleFactory::LoadFromStorage(connection, object, lock);

    auto player = std::dynamic_pointer_cast<Player>(object);
    if(!player)
    {
        throw InvalidObject("Object requested for loading is not Intangible");
    }

    auto statement = std::unique_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetPlayer(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));
  
    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());

    do
    { 
        while (result->next())
        {
            // Player specific start
            player->SetProfessionTag(result->getString("profession_tag"), lock);
            player->SetBornDate(result->getUInt("born_date"), lock);
            player->SetTotalPlayTime(result->getUInt("total_playtime"), lock);
            player->SetAdminTag(result->getUInt("csr_tag"), lock);
            player->SetCurrentForcePower(result->getUInt("current_force"), lock);
            player->SetMaxForcePower(result->getUInt("max_force"), lock);
            player->SetExperimentationFlag(result->getUInt("experimentation_enabled"), lock);
            player->SetCraftingStage(result->getUInt("crafting_stage"), lock);
            player->SetNearestCraftingStation(result->getUInt64("nearest_crafting_station"), lock);
            player->AddExperimentationPoints(result->getUInt("experimentation_points"), lock);
            player->ResetAccomplishmentCounter(result->getUInt("accomplishment_counter"), lock);
            player->SetLanguage(result->getUInt("current_language"), lock);
            player->ResetCurrentStomach(result->getUInt("current_stomach"), lock);
            player->ResetMaxStomach(result->getUInt("max_stomach"), lock);
            player->ResetCurrentDrink(result->getUInt("current_drink"), lock);
            player->ResetMaxDrink(result->getUInt("max_drink"), lock);
            player->SetJediState(result->getUInt("jedi_state"), lock);
        }
    } while(statement->getMoreResults());
    
    LoadStatusFlags_(connection, player, lock);
    LoadProfileFlags_(connection, player, lock);
    LoadBadges_(connection, player, lock);
    LoadDraftSchematics_(connection, player, lock);
    LoadFriends_(connection, player, lock);
    LoadForceSensitiveQuests_(connection, player, lock);
    LoadIgnoredList_(connection, player, lock);
    LoadQuestJournal_(connection, player, lock);
    LoadWaypoints_(connection, player, lock);
    LoadXP_(connection, player, lock);
}

void PlayerFactory::RegisterEventHandlers()
{
    GetEventDispatcher()->Subscribe("Player::StatusBitmask", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Player::ProfileFlag", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Player::ProfessionTag", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Player::BornDate", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Player::TotalPlayTime", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Player::AdminTag", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Player::Experience", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Player::Waypoint", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Player::ForcePower", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Player::MaxForcePower", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::ForceSensitiveQuests", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::CompletedForceSensitiveQuests", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::QuestJournal", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::ExperimentationFlag", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::CraftingStage", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::NearestCraftingStation", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::DraftSchematic", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::ExperimentationPoints", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::AccomplishmentCounter", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::Friend", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::RemoveFriend", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::IgnorePlayer", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::RemoveIgnoredPlayer", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::Language", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::CurrentStomach", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::MaxStomach", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::CurrentDrink", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::MaxDrink", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));	
	GetEventDispatcher()->Subscribe("Player::JediState", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Player::Badges", std::bind(&PlayerFactory::PersistHandler, this, std::placeholders::_1));
}

void PlayerFactory::PersistChangedObjects()
{
	std::set<shared_ptr<Object>> persisted;
	{
		boost::lock_guard<boost::mutex> lg(persisted_objects_mutex_);
		persisted = move(persisted_objects_);
	}
	for (auto& object : persisted)
	{
		auto lock = object->AcquireLock();
		if(object->IsDatabasePersisted(lock))
		{
			PersistObject(object, lock);
		}
	}
}

uint32_t PlayerFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	uint32_t counter = 1;
	
    IntangibleFactory::PersistObject(object, lock, persist_inherited);

    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
			(conn->prepareStatement("CALL sp_PersistPlayer(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
        
		auto player = static_pointer_cast<Player>(object);
		statement->setUInt64(counter++, player->GetObjectId(lock));
		statement->setString(counter++, player->GetProfessionTag(lock));
		statement->setUInt64(counter++, player->GetTotalPlayTime(lock));
		statement->setUInt(counter++, player->GetAdminTag(lock));
		statement->setUInt(counter++, player->GetMaxForcePower(lock));
		statement->setUInt(counter++, player->GetExperimentationFlag(lock));
		statement->setUInt(counter++, player->GetCraftingStage(lock));
		statement->setUInt64(counter++, player->GetNearestCraftingStation(lock));
		statement->setUInt(counter++, player->GetExperimentationPoints(lock));
		statement->setUInt(counter++, player->GetAccomplishmentCounter(lock));
		statement->setUInt(counter++, player->GetLanguage(lock));
		statement->setUInt(counter++, player->GetCurrentStomach(lock));
		statement->setUInt(counter++, player->GetMaxStomach(lock));
		statement->setUInt(counter++, player->GetCurrentDrink(lock));
		statement->setUInt(counter++, player->GetMaxDrink(lock));
		statement->setUInt(counter++, player->GetJediState(lock));

        statement->executeUpdate();

        PersistFriends_(player, lock);
        PersistIgnoredList_(player, lock);
        PersistXP_(player, lock);
        PersistDraftSchematics_(player, lock);
        PersistForceSensitiveQuests_(player, lock);
        PersistQuestJournal_(player, lock);
		PersistBadges_(player, lock);
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
	return counter;
}

void PlayerFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
    ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> PlayerFactory::CreateObject()
{
	//@TODO: Create me with help from db
	return make_shared<Player>();
}

void PlayerFactory::PersistXP_(const shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto xp = player->GetXp(lock);
        for(auto& xpData : xp)
        {
            auto statement = conn->prepareStatement("CALL sp_UpdateExperience(?,?);");
            statement->setString(1,xpData.first);
            statement->setUInt(2,xpData.second.value);
            auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
        };
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::PersistBadges_(const shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock)
{
	try
	{
		auto conn = GetDatabaseManager()->getConnection("galaxy");
		auto badges = player->GetBadgesSyncQueue(lock);

		while(badges.size())
		{
			auto queue_item = badges.front();

			switch(queue_item.first)
			{
			case 0: // Remove
				{
					auto statement = conn->prepareStatement("CALL sp_RemoveBadge(?, ?);");
					statement->setUInt64(1, player->GetObjectId(lock));
					statement->setUInt(2, queue_item.second);
					auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
					break;
				}

			case 1: // Add
				{
					auto statement = conn->prepareStatement("CALL sp_UpdateBadges(?, ?);");
					statement->setUInt64(1, player->GetObjectId(lock));
					statement->setUInt(2, queue_item.second);
					auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
					break;
				}
			}

			badges.pop();
		}

		//for(auto& badge_id : badges)
		//{
		//	auto statement = conn->prepareStatement("CALL sp_UpdateBadges(?, ?);");
		//	statement->setUInt64(1, player->GetObjectId());
		//	statement->setUInt(2, badge_id);
		//	auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
		//};
	}
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::PersistDraftSchematics_(const shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto draft_schematics = player->GetDraftSchematics(lock);
        for(auto& schematic : draft_schematics)
        {
            auto statement = conn->prepareStatement("CALL sp_UpdateDraftSchematic(?,?,?);");
            statement->setUInt64(1, player->GetObjectId(lock));
            statement->setUInt(2,schematic.schematic_id);
            statement->setUInt(3, schematic.schematic_crc);
            auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
        };
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::PersistQuestJournal_(const shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto quests = player->GetQuests(lock);
        
        for(auto& quest : quests)
        {
            auto statement = conn->prepareStatement("CALL sp_UpdateQuestJournal(?,?,?,?,?,?);");
            statement->setUInt64(1, player->GetObjectId(lock));
            statement->setUInt64(2, quest.second.owner_id);
            statement->setUInt(3, quest.second.quest_crc);
            statement->setUInt(4, quest.second.active_step_bitmask);
            statement->setUInt(5, quest.second.completed_step_bitmask);
            statement->setUInt(6, quest.second.completed_flag);
            
            auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
        };
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::PersistForceSensitiveQuests_(const shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_UpdateFSQuests(?,?,?);");
        statement->setUInt64(1, player->GetObjectId(lock));
        statement->setUInt(2, player->GetCurrentForceSensitiveQuests(lock));
        statement->setUInt(3, player->GetCompletedForceSensitiveQuests(lock));

        auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::RemoveFriend_(const std::shared_ptr<Player>& player, uint64_t friend_id)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        
        auto statement = conn->prepareStatement("CALL sp_RemoveFriend(?,?);");
        statement->setUInt64(1, player->GetObjectId());
        statement->setUInt64(2, friend_id);

        statement->execute();
        }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistFriends_(const shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        for(auto& friend_name : player->GetFriends(lock))
        {
			uint64_t friend_id = character_provider_->GetCharacterIdByName(friend_name);
			if(friend_id != 0)
			{
				auto statement = conn->prepareStatement("CALL sp_UpdateFriends(?,?);");
				statement->setUInt64(1, player->GetObjectId(lock));
				statement->setUInt64(2, friend_id);
				auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
			}
        };
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::PersistIgnoredList_(const shared_ptr<Player>& player, boost::unique_lock<boost::mutex>& lock)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        for(auto& player_name : player->GetIgnoredPlayers(lock))
        {
			uint64_t ignore_id = character_provider_->GetCharacterIdByName(player_name);
			if(ignore_id != 0)
			{
				auto statement = conn->prepareStatement("CALL sp_UpdateIgnoreList(?,?);");
				statement->setUInt64(1, player->GetObjectId(lock));
				statement->setUInt64(2, ignore_id);
				auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
			}
        };
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::RemoveFromIgnoredList_(const shared_ptr<Player>& player, uint64_t ignore_player_id)
{
    try 
    {
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        
        auto statement = conn->prepareStatement("CALL sp_RemoveIgnoredPlayer(?,?);");
        statement->setUInt64(1, player->GetObjectId());
        statement->setUInt64(2, ignore_player_id);

        statement->execute();
        }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::LoadStatusFlags_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
									 boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetPlayerStatusFlags(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            player->AddStatusFlag(static_cast<StatusFlags>(result->getUInt("flag")), lock);
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadProfileFlags_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
									  boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetPlayerProfileFlags(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    { 
        while (result->next())
        {
            player->AddProfileFlag(static_cast<ProfileFlags>(result->getUInt("flag")), lock);
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadBadges_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
								boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetPlayerBadges(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());  
    do
    {  
        while (result->next())
        {
            player->AddBadge(result->getUInt("badge"), lock);
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadDraftSchematics_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
										 boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetPlayerDraftSchematics(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            DraftSchematicData data;
            data.schematic_id = result->getUInt("id");
            data.schematic_crc = result->getUInt("schematic_id");
            // didn't move here because you can't get faster than copying 2 ints
            player->AddDraftSchematic(data, lock);
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadFriends_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
								 boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetFriends(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            player->AddFriend(result->getString("custom_name"), lock);
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadForceSensitiveQuests_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
											  boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetFSQuestList(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {  
        while (result->next())
        {
            if (result->getUInt("completed") == 1)
            {
                player->AddCompletedForceSensitiveQuest(result->getUInt("quest_mask"), lock);
            }
            else
            {
                player->AddCurrentForceSensitiveQuest(result->getUInt("quest_mask"), lock);
            }
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadIgnoredList_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
									 boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetIgnoredList(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {  
        while (result->next())
        {
            player->IgnorePlayer(result->getString("custom_name"), lock);
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadQuestJournal_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
									  boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetQuestJournal(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            QuestJournalData data;
            data.owner_id = result->getUInt64("quest_owner_id");
            data.quest_crc = result->getUInt("quest_crc");
            data.active_step_bitmask = result->getUInt("active_step_bitmask");
            data.completed_step_bitmask = result->getUInt("completed_step_bitmask");
            data.completed_flag = result->getUInt("completed") == 1;
            player->AddQuest(std::move(data), lock);
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadWaypoints_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
								   boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetWaypoints(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {   
        while (result->next())
        {
            if(auto waypoint = object_manager_->CreateObjectFromStorage<Waypoint>(result->getUInt64("id")))
            {
                player->AddWaypoint(waypoint, lock);
            }
        }
    } while(statement->getMoreResults());
}

void PlayerFactory::LoadXP_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Player>& player,
							boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetExperienceList(?);"));
    
    statement->setUInt64(1, player->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    { 
        while (result->next())
        {
            player->AddExperience(result->getString("name"), result->getUInt("value"), lock);
        }
    } while(statement->getMoreResults());
}
