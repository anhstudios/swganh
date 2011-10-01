
#include "swganh/object/player/player_factory.h"

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/player/player.h"
#include "swganh/object/exception.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::player;
using namespace swganh::object::waypoint;

PlayerFactory::PlayerFactory(const shared_ptr<DatabaseManagerInterface>& db_manager)
    : db_manager_(db_manager)
{
}

void PlayerFactory::LoadTemplates()
{
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetPlayerTemplates();");
        auto result = statement->executeQuery();

        while (result->next())
        {
            auto player = make_shared<Player>();
            player->SetPosition(glm::vec3(result->getDouble(1),result->getDouble(2), result->getDouble(3)));
            player->SetOrientation(glm::quat(result->getDouble(4),result->getDouble(5), result->getDouble(6), result->getDouble(7)));
            player->SetComplexity(result->getDouble(8));
            player->SetStfNameFile(result->getString(9));
            player->SetStfNameString(result->getString(10));
            string custom_string = result->getString(11);
            player->SetCustomName(wstring(begin(custom_string), end(custom_string)));
            player->SetVolume(result->getUInt(12));
            // Player specific start
            player->AddStatusFlag(result->getUInt(13));
            player->AddProfileFlag(result->getUInt(14));
            player->SetProfessionTag(result->getString(15));
            player->SetBornDate(result->getUInt(16));
            player->SetTotalPlayTime(result->getUInt(17));
            player->SetAdminTag(result->getUInt(18));
            player->SetRegionId(result->getUInt(19));
            // TODO: XP
            // TODO: Waypoints
            player->SetCurrentForcePower(result->getUInt(20));
            player->SetMaxForcePower(result->getUInt(21));
            player->AddCurrentForceSensitiveQuest(result->getUInt(22));
            player->AddCompletedForceSensitiveQuest(result->getUInt(23));
            // TODO: Quests
            // TODO: Abilities
            player->SetExperimentationFlag(result->getUInt(24));
            player->SetCraftingStage(result->getUInt(25));
            player->SetNearestCraftingStation(result->getUInt64(26));
            // TODO: Draft Schematics
            player->AddExperimentationPoints(result->getUInt(27));
            player->ResetAccomplishmentCounter(result->getUInt(28));
            // TODO: Friends
            // TODO: Ignored
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
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
    if (object->IsDirty())
    {
        try 
        {
            auto conn = db_manager_->getConnection("galaxy");
            auto statement = conn->prepareStatement("CALL sp_PersistPlayer(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");
            // TODO: Add in values to persist 33 minus lists
            statement->execute();
        }
            catch(sql::SQLException &e)
        {
            DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
            DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        }
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
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

shared_ptr<Object> PlayerFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto player = make_shared<Player>();
    try {

        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetPlayer(?);");
        statement->setUInt64(1, object_id);
        auto result = statement->executeQuery();
        while (result->next())
        {
            player->SetPosition(glm::vec3(result->getDouble(1),result->getDouble(2), result->getDouble(3)));
            player->SetOrientation(glm::quat(result->getDouble(4),result->getDouble(5), result->getDouble(6), result->getDouble(7)));
            player->SetComplexity(result->getDouble(8));
            player->SetStfNameFile(result->getString(9));
            player->SetStfNameString(result->getString(10));
            string custom_string = result->getString(11);
            player->SetCustomName(wstring(begin(custom_string), end(custom_string)));
            player->SetVolume(result->getUInt(12));
            // Player specific start
            player->AddStatusFlag(result->getUInt(13));
            player->AddProfileFlag(result->getUInt(14));
            player->SetProfessionTag(result->getString(15));
            player->SetBornDate(result->getUInt(16));
            player->SetTotalPlayTime(result->getUInt(17));
            player->SetAdminTag(result->getUInt(18));
            player->SetRegionId(result->getUInt(19));
            LoadXP_(player);
            LoadWaypoints_(player);
            player->SetCurrentForcePower(result->getUInt(20));
            player->SetMaxForcePower(result->getUInt(21));
            player->AddCurrentForceSensitiveQuest(result->getUInt(22));
            player->AddCompletedForceSensitiveQuest(result->getUInt(23));
            LoadQuestJournal_(player);
            LoadAbilities_(player);
            player->SetExperimentationFlag(result->getUInt(24));
            player->SetCraftingStage(result->getUInt(25));
            player->SetNearestCraftingStation(result->getUInt64(26));
            LoadDraftSchematics_(player);
            player->AddExperimentationPoints(result->getUInt(27));
            player->ResetAccomplishmentCounter(result->getUInt(28));
            LoadFriends_(player);
            LoadIgnoredList_(player);
            player->ResetCurrentStomach(result->getUInt(29));
            player->ResetMaxStomach(result->getUInt(30));
            player->ResetCurrentDrink(result->getUInt(31));
            player->ResetMaxDrink(result->getUInt(32));
            player->SetJediState(result->getUInt(33));
        }
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
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
// Helpers
void PlayerFactory::LoadXP_(std::shared_ptr<Player> player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetExperience(?);");
        statement->setUInt64(1, player->GetObjectId());
        auto result = statement->executeQuery();
        while (result->next())
        {
            player->AddExperience(result->getString("type"), result->getUInt("value"));

        } while (statement->getMoreResults());
    }
        catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::PersistXP_(const std::shared_ptr<Player>& player)
{
    try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto xp = player->GetXp();
        for_each(begin(xp), end(xp), [this,&conn] (Player::XpData xpData){
            auto statement = conn->prepareStatement("CALL sp_SaveExperience(?,?);");
            statement->setString(1,xpData.type);
            statement->setUInt(2,xpData.value);
            auto result = statement->executeQuery();
        });
    }
        catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}
void PlayerFactory::LoadWaypoints_(std::shared_ptr<Player> player)
{
    // Call Waypoint Factory??
    /*try 
    {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetWaypoints(?);");
        statement->setUInt64(1, player->GetObjectId());
        auto result = statement->executeQuery();
        while (result->next())
        {
            

        } while (statement->GetMoreResults());
    }
        catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }*/
}
void PlayerFactory::PersistWaypoints_(const std::shared_ptr<Player>& player)
{
    // Call Waypoint Factory??
}
void PlayerFactory::LoadDraftSchematics_(std::shared_ptr<Player> player)
{
    // Call Draft Schematics Factory??
}
void PlayerFactory::PersistDraftSchematics_(const std::shared_ptr<Player>& player)
{
    // Call Draft Schematics Factory??
}
void PlayerFactory::LoadQuestJournal_(std::shared_ptr<Player> player)
{
    // ETC
}
void PlayerFactory::PersistQuestJournal_(const std::shared_ptr<Player>& player)
{
}
void PlayerFactory::LoadAbilities_(std::shared_ptr<Player> player)
{
}
void PlayerFactory::PersistAbilities_(const std::shared_ptr<Player>& player)
{
}
void PlayerFactory::LoadFriends_(std::shared_ptr<Player> player)
{
}
void PlayerFactory::PersistFriends_(const std::shared_ptr<Player>& player)
{
}
void PlayerFactory::LoadIgnoredList_(std::shared_ptr<Player> player)
{
}
void PlayerFactory::PersistIgnoredList_(const std::shared_ptr<Player>& player)
{
}
