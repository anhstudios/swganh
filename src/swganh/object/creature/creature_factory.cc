
#include "swganh/object/creature/creature_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "anh/logger.h"

#include "anh/database/database_manager.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::simulation;


CreatureFactory::CreatureFactory(DatabaseManagerInterface* db_manager,
                             SimulationService* simulation_service,
                             anh::EventDispatcher* event_dispatcher)
    : TangibleFactory(db_manager, simulation_service, event_dispatcher)
{
}

void CreatureFactory::LoadTemplates()
{}

bool CreatureFactory::HasTemplate(const string& template_name)
{
    return false;
}

void CreatureFactory::PersistObject(const shared_ptr<Object>& object)
{
    // Persist Tangible and Base Object First
    TangibleFactory::PersistObject(object);
    // Now for the biggy
    try
    {
        auto conn = db_manager_->getConnection("galaxy");
        // 65 of these
        string sql = "CALL sp_PersistCreature(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,"
            "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
        auto statement = unique_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        auto creature = static_pointer_cast<Creature>(object);
        statement->setUInt64(1, creature->GetObjectId());
        statement->setUInt64(2, creature->GetOwnerId());
        statement->setUInt64(3, creature->GetListenToId());
        statement->setUInt64(4, creature->GetBankCredits());
        statement->setUInt64(5, creature->GetCashCredits());
        statement->setUInt64(6, creature->GetPosture());
        statement->setUInt(7, creature->GetFactionRank());
        statement->setDouble(8, creature->GetScale());
        statement->setUInt64(9, creature->GetBattleFatigue());
        statement->setUInt64(10, creature->GetStateBitmask());
        statement->setDouble(11, creature->GetAccelerationMultiplierBase());
        statement->setDouble(12, creature->GetAccelerationMultiplierModifier());
        statement->setDouble(13, creature->GetSpeedMultiplierBase());
        statement->setDouble(14, creature->GetSpeedMultiplierModifier());
        statement->setDouble(15, creature->GetRunSpeed());
        statement->setDouble(16, creature->GetSlopeModifierAngle());
        statement->setDouble(17, creature->GetSlopeModifierPercent());
        statement->setDouble(18, creature->GetTurnRadius());
        statement->setDouble(19, creature->GetWalkingSpeed());
        statement->setDouble(20, creature->GetWaterModifierPercent());
        statement->setInt(21, creature->GetCombatLevel());
        statement->setString(22, creature->GetAnimation());
        statement->setUInt64(23, creature->GetGroupId());
        statement->setUInt(24, creature->GetGuildId());
        statement->setUInt64(25, creature->GetWeaponId());
        statement->setUInt(26, creature->GetMoodId());
        statement->setUInt(27, creature->GetPerformanceId());
        statement->setString(28, creature->GetDisguise());
        // WOUNDS
        statement->setInt(29, creature->GetStatWound(HEALTH));
        statement->setInt(30, creature->GetStatWound(STRENGTH));
        statement->setInt(31, creature->GetStatWound(CONSTITUTION));
        statement->setInt(32, creature->GetStatWound(ACTION));
        statement->setInt(33, creature->GetStatWound(QUICKNESS));
        statement->setInt(34, creature->GetStatWound(STAMINA));
        statement->setInt(35, creature->GetStatWound(MIND));
        statement->setInt(36, creature->GetStatWound(FOCUS));
        statement->setInt(37, creature->GetStatWound(WILLPOWER));
        // ENCUMBERANCE
        statement->setInt(38, creature->GetStatEncumberance(HEALTH));
        statement->setInt(39, creature->GetStatEncumberance(STRENGTH));
        statement->setInt(40, creature->GetStatEncumberance(CONSTITUTION));
        statement->setInt(41, creature->GetStatEncumberance(ACTION));
        statement->setInt(42, creature->GetStatEncumberance(QUICKNESS));
        statement->setInt(43, creature->GetStatEncumberance(STAMINA));
        statement->setInt(44, creature->GetStatEncumberance(MIND));
        statement->setInt(45, creature->GetStatEncumberance(FOCUS));
        statement->setInt(46, creature->GetStatEncumberance(WILLPOWER));
        // CURRENT
        statement->setInt(47, creature->GetStatCurrent(HEALTH));
        statement->setInt(48, creature->GetStatCurrent(STRENGTH));
        statement->setInt(49, creature->GetStatCurrent(CONSTITUTION));
        statement->setInt(50, creature->GetStatCurrent(ACTION));
        statement->setInt(51, creature->GetStatCurrent(QUICKNESS));
        statement->setInt(52, creature->GetStatCurrent(STAMINA));
        statement->setInt(53, creature->GetStatCurrent(MIND));
        statement->setInt(54, creature->GetStatCurrent(FOCUS));
        statement->setInt(55, creature->GetStatCurrent(WILLPOWER));
        // MAX
        statement->setInt(56, creature->GetStatMax(HEALTH));
        statement->setInt(57, creature->GetStatMax(STRENGTH));
        statement->setInt(58, creature->GetStatMax(CONSTITUTION));
        statement->setInt(59, creature->GetStatMax(ACTION));
        statement->setInt(60, creature->GetStatMax(QUICKNESS));
        statement->setInt(61, creature->GetStatMax(STAMINA));
        statement->setInt(62, creature->GetStatMax(MIND));
        statement->setInt(63, creature->GetStatMax(FOCUS));
        statement->setInt(64, creature->GetStatMax(WILLPOWER));

        int updated = statement->executeUpdate();
        LOG(warning) << "Updated " << updated << " rows in sp_PersistCreature";
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

void CreatureFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> CreatureFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto creature = make_shared<Creature>();
    creature->SetObjectId(object_id);
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        unique_ptr<sql::ResultSet> result;
        stringstream ss;
        ss << "CALL sp_GetCreature(" << object_id << ");" ;
        statement->execute(ss.str());
        CreateTangible(creature, statement);
        
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());
            while (result->next())
            {
                creature->SetOwnerId(result->getUInt64("owner_id"));
                creature->SetListenToId(result->getUInt64("musician_id"));
                creature->SetBankCredits(result->getUInt("bank_credits"));
                creature->SetCashCredits(result->getUInt("cash_credits"));
                creature->SetPosture((Posture)result->getUInt("posture"));
                creature->SetFactionRank(result->getUInt("faction_rank"));
                creature->SetScale(static_cast<float>(result->getDouble("scale")));
                creature->SetBattleFatigue(result->getUInt("battle_fatigue"));
                creature->SetStateBitmask(result->getUInt("state"));
                creature->SetAccelerationMultiplierBase(static_cast<float>(result->getDouble("acceleration_base")));
                creature->SetAccelerationMultiplierModifier(static_cast<float>(result->getDouble("acceleration_modifier")));
                creature->SetSpeedMultiplierBase(static_cast<float>(result->getDouble("speed_base")));
                creature->SetSpeedMultiplierModifier(static_cast<float>(result->getDouble("speed_modifier")));
                creature->SetRunSpeed(static_cast<float>(result->getDouble("run_speed")));
                creature->SetSlopeModifierAngle(static_cast<float>(result->getDouble("slope_modifier_angle")));
                creature->SetSlopeModifierPercent(static_cast<float>(result->getDouble("slope_modifier_percent")));
                creature->SetWalkingSpeed(static_cast<float>(result->getDouble("walking_speed")));
                creature->SetTurnRadius(static_cast<float>(result->getDouble("turn_radius")));
                creature->SetWaterModifierPercent(static_cast<float>(result->getDouble("water_modifier_percent")));
                creature->SetCombatLevel(result->getUInt("combat_level"));
                creature->SetAnimation(result->getString("animation"));
                creature->SetMoodAnimation(result->getString("mood_animation"));

                /// @TODO: Find a better place for this.
                if (creature->GetMoodAnimation().compare("none") == 0)
                {
                    creature->SetMoodAnimation("neutral");
                }

                creature->SetGroupId(result->getUInt64("group_id"));
                creature->SetGuildId(result->getUInt("guild_id"));
                creature->SetWeaponId(result->getUInt64("weapon_id"));
                creature->SetMoodId(result->getUInt("mood_id"));
                creature->SetPerformanceId(result->getUInt("performance_id"));
                creature->SetDisguise(result->getString("disguise_template"));

                creature->SetStatCurrent(HEALTH, result->getUInt("current_health"));
                creature->SetStatCurrent(STRENGTH, result->getUInt("current_strength"));
                creature->SetStatCurrent(CONSTITUTION, result->getUInt("current_constitution"));
                creature->SetStatCurrent(ACTION, result->getUInt("current_action"));
                creature->SetStatCurrent(QUICKNESS, result->getUInt("current_quickness"));
                creature->SetStatCurrent(STAMINA, result->getUInt("current_stamina"));
                creature->SetStatCurrent(MIND, result->getUInt("current_mind"));
                creature->SetStatCurrent(FOCUS, result->getUInt("current_focus"));
                creature->SetStatCurrent(WILLPOWER, result->getUInt("current_willpower"));

                creature->SetStatMax(HEALTH, result->getUInt("max_health"));
                creature->SetStatMax(STRENGTH, result->getUInt("max_strength"));
                creature->SetStatMax(CONSTITUTION, result->getUInt("max_constitution"));
                creature->SetStatMax(ACTION, result->getUInt("max_action"));
                creature->SetStatMax(QUICKNESS, result->getUInt("max_quickness"));
                creature->SetStatMax(STAMINA, result->getUInt("max_stamina"));
                creature->SetStatMax(MIND, result->getUInt("max_mind"));
                creature->SetStatMax(FOCUS, result->getUInt("max_focus"));
                creature->SetStatMax(WILLPOWER, result->getUInt("max_willpower"));

                creature->SetStatWound(HEALTH, result->getUInt("health_wounds"));
                creature->SetStatWound(STRENGTH, result->getUInt("strength_wounds"));
                creature->SetStatWound(CONSTITUTION, result->getUInt("constitution_wounds"));
                creature->SetStatWound(ACTION, result->getUInt("action_wounds"));
                creature->SetStatWound(QUICKNESS, result->getUInt("quickness_wounds"));
                creature->SetStatWound(STAMINA, result->getUInt("stamina_wounds"));
                creature->SetStatWound(MIND, result->getUInt("mind_wounds"));
                creature->SetStatWound(FOCUS, result->getUInt("focus_wounds"));
                creature->SetStatWound(WILLPOWER, result->getUInt("willpower_wounds"));

                creature->SetStatBase(HEALTH, result->getUInt("health_wounds"));
                creature->SetStatBase(STRENGTH, result->getUInt("strength_wounds"));
                creature->SetStatBase(CONSTITUTION, result->getUInt("constitution_wounds"));
                creature->SetStatBase(ACTION, result->getUInt("action_wounds"));
                creature->SetStatBase(QUICKNESS, result->getUInt("quickness_wounds"));
                creature->SetStatBase(STAMINA, result->getUInt("stamina_wounds"));
                creature->SetStatBase(MIND, result->getUInt("mind_wounds"));
                creature->SetStatBase(FOCUS, result->getUInt("focus_wounds"));
                creature->SetStatBase(WILLPOWER, result->getUInt("willpower_wounds"));
            }
        }
        
        LoadSkills_(creature, statement);
        LoadSkillMods_(creature, statement);
        LoadSkillCommands_(creature, statement);

        LoadContainedObjects(creature, statement);
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return creature;
}

shared_ptr<Object> CreatureFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Creature>();
}

void CreatureFactory::LoadSkills_(
    const shared_ptr<Creature>& creature, 
    const shared_ptr<sql::Statement>& statement)
{
    // Check for contained objects        
    if (statement->getMoreResults())
    {
        unique_ptr<sql::ResultSet> result(statement->getResultSet());

        string skill_name;

        while (result->next())
        {
            skill_name = result->getString("name");

            creature->AddSkill(skill_name);
        }
    }
}

void CreatureFactory::LoadSkillMods_(
    const shared_ptr<Creature>& creature, 
    const shared_ptr<sql::Statement>& statement)
{
    // Check for contained objects        
    if (statement->getMoreResults())
    {
        unique_ptr<sql::ResultSet> result(statement->getResultSet());

        string skill_mod_name;
        uint32_t skill_mod_value;

        while (result->next())
        {
            skill_mod_name = result->getString("name");
            skill_mod_value = result->getUInt("value");

            creature->AddSkillMod( 
                SkillMod(skill_mod_name, skill_mod_value, 0)
                );
        }
    }
}

void CreatureFactory::LoadSkillCommands_(
    const shared_ptr<Creature>& creature, 
    const shared_ptr<sql::Statement>& statement)
{
     // Check for contained objects        
    if (statement->getMoreResults())
    {
        unique_ptr<sql::ResultSet> result(statement->getResultSet());
        while (result->next())
        {
           creature->AddSkillCommand(make_pair(result->getInt("id"), result->getString("name")));
        }
    }
}