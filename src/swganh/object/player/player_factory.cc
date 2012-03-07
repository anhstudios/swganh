
#include "swganh/object/player/player_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <boost/log/trivial.hpp>

#include "anh/crc.h"
#include "anh/database/database_manager.h"
#include "swganh/object/player/player.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::player;
using namespace swganh::object::waypoint;
using namespace swganh::simulation;

uint32_t PlayerFactory::GetType() const { return Player::type; }

PlayerFactory::PlayerFactory(DatabaseManagerInterface* db_manager,
                             SimulationService* simulation_service)
    : ObjectFactory(db_manager, simulation_service)
{
}

void PlayerFactory::LoadTemplates()
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetPlayerTemplates();");
        auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());

        while (result->next())
        {
            auto player = make_shared<Player>();
            player->SetPosition(glm::vec3(
                static_cast<float>(result->getDouble(1)),
                static_cast<float>(result->getDouble(2)),
                static_cast<float>(result->getDouble(3))));
            player->SetOrientation(glm::quat(
                static_cast<float>(result->getDouble(4)),
                static_cast<float>(result->getDouble(5)),
                static_cast<float>(result->getDouble(6)),
                static_cast<float>(result->getDouble(7))));
            player->SetComplexity(static_cast<float>(result->getDouble(8)));
            player->SetStfName(result->getString(9), result->getString(10));
            string custom_string = result->getString(11);
            player->SetCustomName(wstring(begin(custom_string), end(custom_string)));
            player->SetVolume(result->getUInt(12));
            // Player specific start
            player->AddStatusFlag((StatusFlags)result->getUInt(13));
            player->AddProfileFlag((ProfileFlags)result->getUInt(14));
            player->SetProfessionTag(result->getString(15));
            player->SetBornDate(result->getUInt(16));
            player->SetTotalPlayTime(result->getUInt(17));
            player->SetAdminTag(result->getUInt(18));
            //@TODO: XP
            //@TODO: Waypoints
            player->SetCurrentForcePower(result->getUInt(20));
            player->SetMaxForcePower(result->getUInt(21));
            player->AddCurrentForceSensitiveQuest(result->getUInt(22));
            player->AddCompletedForceSensitiveQuest(result->getUInt(23));
            //@TODO: Quests
            //@TODO: Abilities
            player->SetExperimentationFlag(result->getUInt(24));
            player->SetCraftingStage(result->getUInt(25));
            player->SetNearestCraftingStation(result->getUInt64(26));
            //@TODO: Draft Schematics
            player->AddExperimentationPoints(result->getUInt(27));
            player->ResetAccomplishmentCounter(result->getUInt(28));
            //@TODO: Friends
            //@TODO: Ignored
            player->ResetCurrentStomach(result->getUInt(29));
            player->ResetMaxStomach(result->getUInt(30));
            player->ResetCurrentDrink(result->getUInt(31));
            player->ResetMaxDrink(result->getUInt(32));
            player->SetJediState(result->getUInt(33));
            
            player_templates_.insert(make_pair(player->GetTemplate(), move(player)));
        } while (statement->getMoreResults());
    }
    catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
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
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
    }
    catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
               player->AddStatusFlag(static_cast<StatusFlags>(result->getUInt("flag")));
            }
        }
    }
    catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void PlayerFactory::LoadProfileFlags_(std::shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
               player->AddProfileFlag(static_cast<ProfileFlags>(result->getUInt("flag")));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

// Helpers
void PlayerFactory::LoadXP_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                player->AddExperience(XpData(result->getString("name"), result->getUInt("value")));

            }
        }
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistXP_(const shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto xp = player->GetXp();
        for_each(xp.Begin(), xp.End(), [this,&conn] (pair<string, XpData> xpData){
            auto statement = conn->prepareStatement("CALL sp_SaveExperience(?,?);");
            statement->setString(1,xpData.first);
            statement->setUInt(2,xpData.second.value);
            auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
        });
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::LoadWaypoints_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                // Check to see if the waypoint is already available?
                WaypointData waypoint;
                waypoint.object_id_ = result->getUInt64("id");
                waypoint.coordinates_ = glm::vec3(
                    result->getDouble("x_position"),
                    result->getDouble("y_position"), 
                    result->getDouble("z_position"));
                waypoint.location_network_id_ = result->getUInt("scene_id");
                string custom_string = result->getString("custom_name");
                waypoint.name_ = wstring(begin(custom_string), end(custom_string));
                waypoint.activated_flag_ = result->getUInt("is_active");
                waypoint.color_ = result->getUInt("color");
            
                player->AddWaypoint(move(waypoint));
            }
        }
    }
    catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistWaypoints_(const shared_ptr<Player>& player)
{
    // Call Waypoint Factory??
}
void PlayerFactory::LoadDraftSchematics_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                DraftSchematicData data;
                data.schematic_id = result->getUInt("id");
                data.schematic_crc = result->getUInt("schematic");
                // didn't move here because you can't get faster than copying 2 ints
                player->AddDraftSchematic(data);
            }
        }
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistDraftSchematics_(const shared_ptr<Player>& player)
{
    // Call Draft Schematics Factory??
}
void PlayerFactory::LoadQuestJournal_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                QuestJournalData data;
                data.owner_id = result->getUInt64("quest_owner_id");
                data.quest_crc = anh::memcrc(result->getString("name"));
                data.active_step_bitmask = result->getUInt("active_step_bitmask");
                data.completed_step_bitmask = result->getUInt("completed_step_bitmask");
                data.completed_flag = result->getUInt("completed") == 1;
                player->AddQuest(move(data));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistQuestJournal_(const shared_ptr<Player>& player)
{

}
void PlayerFactory::LoadForceSensitiveQuests_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
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
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistForceSensitiveQuests_(const shared_ptr<Player>& player)
{
}

void PlayerFactory::LoadFriends_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
               player->AddFriend(result->getString("custom_name"));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistFriends_(const shared_ptr<Player>& player)
{
}
void PlayerFactory::LoadIgnoredList_(shared_ptr<Player> player, const std::shared_ptr<sql::Statement>& statement)
{
    try 
    {
        auto result = unique_ptr<sql::ResultSet>(statement->getResultSet());
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
               player->IgnorePlayer(result->getString("custom_name"));
            }
        }
    }
        catch(sql::SQLException &e)
    {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistIgnoredList_(const shared_ptr<Player>& player)
{
}
