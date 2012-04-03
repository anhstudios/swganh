
#include "swganh/object/player/player_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "anh/logger.h"

#include "anh/crc.h"
#include "anh/database/database_manager.h"
#include "swganh/object/player/player.h"
#include "swganh/object/player/player_events.h"

#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::player;
using namespace swganh::simulation;

uint32_t PlayerFactory::GetType() const { return Player::type; }

PlayerFactory::PlayerFactory(anh::database::DatabaseManagerInterface* db_manager,
            swganh::simulation::SimulationService* simulation_service,
            anh::EventDispatcher* event_dispatcher)
    : ObjectFactory(db_manager, simulation_service, event_dispatcher)
{
    RegisterEventHandlers();
}

void PlayerFactory::RegisterEventHandlers()
{
    event_dispatcher_->Subscribe("Player::RemoveFriend", [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        auto name_event = static_pointer_cast<NameEvent>(incoming_event);
        RemoveFriend_(name_event->player, name_event->name_id);
    });

    event_dispatcher_->Subscribe("Player::RemoveIgnoredPlayer", [this] (shared_ptr<anh::EventInterface> incoming_event)
    {
        auto name_event = static_pointer_cast<NameEvent>(incoming_event);
        RemoveFromIgnoredList_(name_event->player, name_event->name_id);
    });
}

void PlayerFactory::LoadTemplates()
{
}

bool PlayerFactory::HasTemplate(const string& template_name)
{
    return GetTemplateIter_(template_name) != end(player_templates_);
}
unordered_map<string,shared_ptr<Player>>::iterator PlayerFactory::GetTemplateIter_(const string& template_name)
{
    auto iter = find_if(begin(player_templates_), end(player_templates_), [&template_name] (pair<string, shared_ptr<Player>> template_pair) {
        return template_name == template_pair.first;
    });

    return iter;
}

void PlayerFactory::PersistObject(const shared_ptr<Object>& object)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::PreparedStatement>
			(conn->prepareStatement("CALL sp_PersistPlayer(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
        ObjectFactory::PersistObject(object, statement);

		auto player = static_pointer_cast<Player>(object);
		statement->setString(17, player->profession_tag_);
		statement->setUInt64(18, player->total_playtime_);
		statement->setUInt(19, player->admin_tag_);
		statement->setUInt(20, player->max_force_power_);
		statement->setUInt(21, player->experimentation_flag_);
		statement->setUInt(22, player->crafting_stage_);
		statement->setUInt64(23, player->nearest_crafting_station_);
		statement->setUInt(24, player->experimentation_points_);
		statement->setUInt(25, player->accomplishment_counter_);
		statement->setUInt(26, player->language_);
		statement->setUInt(27, player->current_stomach_);
		statement->setUInt(28, player->max_stomach_);
		statement->setUInt(29, player->current_drink_);
		statement->setUInt(30, player->max_drink_);
		statement->setUInt(31, player->jedi_state_);

        statement->executeUpdate();

        PersistFriends_(player);
        PersistIgnoredList_(player);
        PersistXP_(player);
        PersistDraftSchematics_(player);
        PersistForceSensitiveQuests_(player);
        PersistQuestJournal_(player);
        PersistWaypoints_(player);
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    
}

void PlayerFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_DeletePlayer(?);");
        statement->setUInt64(1, object->GetObjectId());
        statement->execute();
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

shared_ptr<Object> PlayerFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto player = make_shared<Player>();
    player->SetObjectId(object_id);
    try {

        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        
        stringstream ss;
        ss << "CALL sp_GetPlayer(" << object_id << ");";

        auto result = shared_ptr<sql::ResultSet>(statement->executeQuery(ss.str()));
        CreateBaseObjectFromStorage(player, result);
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                // Player specific start
                player->SetProfessionTag(result->getString("profession_tag"));
                player->SetBornDate(result->getUInt("born_date"));
                player->SetTotalPlayTime(result->getUInt("total_playtime"));
                player->SetAdminTag(result->getUInt("csr_tag"));
                player->SetCurrentForcePower(result->getUInt("current_force"));
                player->SetMaxForcePower(result->getUInt("max_force"));
                player->SetExperimentationFlag(result->getUInt("experimentation_enabled"));
                player->SetCraftingStage(result->getUInt("crafting_stage"));
                player->SetNearestCraftingStation(result->getUInt64("nearest_crafting_station"));
                player->AddExperimentationPoints(result->getUInt("experimentation_points"));
                player->ResetAccomplishmentCounter(result->getUInt("accomplishment_counter"));
                player->SetLanguage(result->getUInt("current_language"));
                player->ResetCurrentStomach(result->getUInt("current_stomach"));
                player->ResetMaxStomach(result->getUInt("max_stomach"));
                player->ResetCurrentDrink(result->getUInt("current_drink"));
                player->ResetMaxDrink(result->getUInt("max_drink"));
                player->SetJediState(result->getUInt("jedi_state"));
            }

            LoadStatusFlags_(player, statement);
            LoadProfileFlags_(player, statement);
            LoadDraftSchematics_(player, statement);
            LoadFriends_(player, statement);
            LoadForceSensitiveQuests_(player, statement);
            LoadIgnoredList_(player, statement);
            LoadQuestJournal_(player, statement);
            LoadWaypoints_(player, statement);
            LoadXP_(player, statement);
        }

    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return player;
}

shared_ptr<Object> PlayerFactory::CreateObjectFromTemplate(const string& template_name)
{
    auto object = make_shared<Player>();
    auto found = GetTemplateIter_(template_name);
    if (found != end(player_templates_))
    {
        object = found->second;
    }
    else
    {
        throw swganh::object::InvalidObjectTemplate("Template Not Found: " + template_name);
    }
    
    return object;
}

void PlayerFactory::LoadStatusFlags_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{    
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
               player->AddStatusFlag(static_cast<StatusFlags>(result->getUInt("flag")));
            }
        }
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::LoadProfileFlags_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
               player->AddProfileFlag(static_cast<ProfileFlags>(result->getUInt("flag")));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

// Helpers
void PlayerFactory::LoadXP_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
                player->AddExperience(XpData(result->getString("name"), result->getUInt("value")));

            }
        }
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistXP_(const shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto xp = player->GetXp();
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
void PlayerFactory::LoadWaypoints_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = shared_ptr<sql::ResultSet>(statement->getResultSet());
            event_dispatcher_->Dispatch(make_shared<WaypointEvent>("LoadWaypoints", player, result));
        }
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistWaypoints_(const shared_ptr<Player>& player)
{
    auto waypoints = player->GetWaypoints();
    event_dispatcher_->Dispatch(make_shared<anh::ValueEvent<swganh::messages::containers::NetworkMap<uint64_t, PlayerWaypointSerializer>>>
                                ("PersistWaypoints", waypoints));
}
void PlayerFactory::LoadDraftSchematics_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = shared_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
                DraftSchematicData data;
                data.schematic_id = result->getUInt("id");
                data.schematic_crc = result->getUInt("schematic_id");
                // didn't move here because you can't get faster than copying 2 ints
                player->AddDraftSchematic(data);
            }
        }
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistDraftSchematics_(const shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto draft_schematics = player->GetDraftSchematics();
        for(auto& schematic : draft_schematics)
        {
            auto statement = conn->prepareStatement("CALL sp_UpdateDraftSchematic(?,?,?);");
            statement->setUInt64(1, player->GetObjectId());
            statement->setUInt(2,schematic.second.schematic_id);
            statement->setUInt(3, schematic.second.schematic_crc);
            auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
        };
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::LoadQuestJournal_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = shared_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
                QuestJournalData data;
                data.owner_id = result->getUInt64("quest_owner_id");
                data.quest_crc = result->getUInt("quest_crc");
                data.active_step_bitmask = result->getUInt("active_step_bitmask");
                data.completed_step_bitmask = result->getUInt("completed_step_bitmask");
                data.completed_flag = result->getUInt("completed") == 1;
                player->AddQuest(move(data));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistQuestJournal_(const shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto quests = player->GetQuests();
        
        for(auto& quest : quests)
        {
            auto statement = conn->prepareStatement("CALL sp_UpdateQuestJournal(?,?,?,?,?,?);");
            statement->setUInt64(1, player->GetObjectId());
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
void PlayerFactory::LoadForceSensitiveQuests_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = shared_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
                if (result->getUInt("completed") == 1)
                {
                    player->AddCompletedForceSensitiveQuest(result->getUInt("quest_mask"));
                }
                else
                {
                    player->AddCurrentForceSensitiveQuest(result->getUInt("quest_mask"));
                }
            }
        }
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistForceSensitiveQuests_(const shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_UpdateFSQuests(?,?,?);");
        statement->setUInt64(1, player->GetObjectId());
        statement->setUInt(2, player->GetCurrentForceSensitiveQuests());
        statement->setUInt(3, player->GetCompletedForceSensitiveQuests());

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
        auto conn = db_manager_->getConnection("galaxy");
        
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
void PlayerFactory::PersistFriends_(const shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto friends = player->GetFriends();
        
        for(auto& friend_name : friends)
        {
            auto statement = conn->prepareStatement("CALL sp_UpdateFriends(?,?);");
            statement->setUInt64(1, player->GetObjectId());
            statement->setUInt64(2, friend_name.id);
            auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
        };
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::LoadFriends_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
               player->AddFriend(result->getString("custom_name"), result->getUInt64("id"));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::LoadIgnoredList_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        if (statement->getMoreResults())
        {
            auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
            while (result->next())
            {
               player->IgnorePlayer(result->getString("custom_name"), result->getUInt64("id"));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistIgnoredList_(const shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto ignored_players = player->GetIgnoredPlayers();
        
        for(auto& player_name : ignored_players)
        {
            auto statement = conn->prepareStatement("CALL sp_UpdateIgnoreList(?,?);");
            statement->setUInt64(1, player->GetObjectId());
            statement->setUInt64(2, player_name.id);
            auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
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
        auto conn = db_manager_->getConnection("galaxy");
        
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