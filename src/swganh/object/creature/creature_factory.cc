
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
        statement->setUInt64(1, creature->object_id_);
        statement->setUInt64(2, creature->owner_id_);
        statement->setUInt64(3, creature->listen_to_id_);
        statement->setUInt64(4, creature->bank_credits_);
        statement->setUInt64(5, creature->cash_credits_);
        statement->setUInt64(6, creature->posture_);
        statement->setUInt(7, creature->faction_rank_);
        statement->setDouble(8, creature->scale_);
        statement->setUInt64(9, creature->battle_fatigue_);
        statement->setUInt64(10, creature->state_bitmask_);
        statement->setDouble(11, creature->acceleration_multiplier_base_);
        statement->setDouble(12, creature->acceleration_multiplier_modifier_);
        statement->setDouble(13, creature->speed_multiplier_base_);
        statement->setDouble(14, creature->speed_multiplier_modifier_);
        statement->setDouble(15, creature->run_speed_);
        statement->setDouble(16, creature->slope_modifier_angle_);
        statement->setDouble(17, creature->slope_modifier_percent_);
        statement->setDouble(18, creature->turn_radius_);
        statement->setDouble(19, creature->walking_speed_);
        statement->setDouble(20, creature->water_modifier_percent_);
        statement->setInt(21, creature->combat_level_);
        statement->setString(22, creature->animation_);
        statement->setUInt64(23, creature->group_id_);
        statement->setUInt(24, creature->guild_id_);
        statement->setUInt64(25, creature->weapon_id_);
        statement->setUInt(26, creature->mood_id_);
        statement->setUInt(27, creature->performance_id_);
        statement->setString(28, creature->disguise_);
        // WOUNDS
        statement->setInt(29, creature->stat_wound_list_.At(HEALTH).value);
        statement->setInt(30, creature->stat_wound_list_.At(STRENGTH).value);
        statement->setInt(31, creature->stat_wound_list_.At(CONSTITUTION).value);
        statement->setInt(32, creature->stat_wound_list_.At(ACTION).value);
        statement->setInt(33, creature->stat_wound_list_.At(QUICKNESS).value);
        statement->setInt(34, creature->stat_wound_list_.At(STAMINA).value);
        statement->setInt(35, creature->stat_wound_list_.At(MIND).value);
        statement->setInt(36, creature->stat_wound_list_.At(FOCUS).value);
        statement->setInt(37, creature->stat_wound_list_.At(WILLPOWER).value);
        // ENCUMBERANCE
        statement->setInt(38, creature->stat_encumberance_list_.At(HEALTH).value);
        statement->setInt(39, creature->stat_encumberance_list_.At(STRENGTH).value);
        statement->setInt(40, creature->stat_encumberance_list_.At(CONSTITUTION).value);
        statement->setInt(41, creature->stat_encumberance_list_.At(ACTION).value);
        statement->setInt(42, creature->stat_encumberance_list_.At(QUICKNESS).value);
        statement->setInt(43, creature->stat_encumberance_list_.At(STAMINA).value);
        statement->setInt(44, creature->stat_encumberance_list_.At(MIND).value);
        statement->setInt(45, creature->stat_encumberance_list_.At(FOCUS).value);
        statement->setInt(46, creature->stat_encumberance_list_.At(WILLPOWER).value);
        // CURRENT
        statement->setInt(47, creature->stat_current_list_.At(HEALTH).value);
        statement->setInt(48, creature->stat_current_list_.At(STRENGTH).value);
        statement->setInt(49, creature->stat_current_list_.At(CONSTITUTION).value);
        statement->setInt(50, creature->stat_current_list_.At(ACTION).value);
        statement->setInt(51, creature->stat_current_list_.At(QUICKNESS).value);
        statement->setInt(52, creature->stat_current_list_.At(STAMINA).value);
        statement->setInt(53, creature->stat_current_list_.At(MIND).value);
        statement->setInt(54, creature->stat_current_list_.At(FOCUS).value);
        statement->setInt(55, creature->stat_current_list_.At(WILLPOWER).value);
        // MAX
        statement->setInt(56, creature->stat_max_list_.At(HEALTH).value);
        statement->setInt(57, creature->stat_max_list_.At(STRENGTH).value);
        statement->setInt(58, creature->stat_max_list_.At(CONSTITUTION).value);
        statement->setInt(59, creature->stat_max_list_.At(ACTION).value);
        statement->setInt(60, creature->stat_max_list_.At(QUICKNESS).value);
        statement->setInt(61, creature->stat_max_list_.At(STAMINA).value);
        statement->setInt(62, creature->stat_max_list_.At(MIND).value);
        statement->setInt(63, creature->stat_max_list_.At(FOCUS).value);
        statement->setInt(64, creature->stat_max_list_.At(WILLPOWER).value);

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
                creature->owner_id_ = result->getUInt64("owner_id");
                creature->listen_to_id_ = result->getUInt64("musician_id");
                creature->bank_credits_ = result->getUInt("bank_credits");
                creature->cash_credits_ = result->getUInt("cash_credits");
                creature->posture_ = (Posture)result->getUInt("posture");
                creature->faction_rank_ = result->getUInt("faction_rank");
                creature->scale_ = static_cast<float>(result->getDouble("scale"));
                creature->battle_fatigue_ = result->getUInt("battle_fatigue");
                creature->state_bitmask_ = result->getUInt("state");
                creature->acceleration_multiplier_base_ = static_cast<float>(result->getDouble("acceleration_base"));
                creature->acceleration_multiplier_modifier_ = static_cast<float>(result->getDouble("acceleration_modifier"));
                creature->speed_multiplier_base_ = static_cast<float>(result->getDouble("speed_base"));
                creature->speed_multiplier_modifier_ = static_cast<float>(result->getDouble("speed_modifier"));
                creature->run_speed_ = static_cast<float>(result->getDouble("run_speed"));
                creature->slope_modifier_angle_ = static_cast<float>(result->getDouble("slope_modifier_angle"));
                creature->slope_modifier_percent_ = static_cast<float>(result->getDouble("slope_modifier_percent"));
                creature->walking_speed_ = static_cast<float>(result->getDouble("walking_speed"));
                creature->turn_radius_ = static_cast<float>(result->getDouble("turn_radius"));
                creature->water_modifier_percent_ = static_cast<float>(result->getDouble("water_modifier_percent"));
                creature->combat_level_ = result->getUInt("combat_level");
                creature->animation_ = result->getString("animation");
                creature->mood_animation_ = result->getString("mood_animation");

                /// @TODO: Find a better place for this.
                if (creature->mood_animation_.compare("none") == 0)
                {
                    creature->mood_animation_ = "neutral";
                }

                creature->group_id_ = result->getUInt64("group_id");
                creature->guild_id_ = result->getUInt("guild_id");
                creature->weapon_id_ = result->getUInt64("weapon_id");
                creature->mood_id_ = result->getUInt("mood_id");
                creature->performance_id_ = result->getUInt("performance_id");
                creature->disguise_ = result->getString("disguise_template");

                creature->stat_current_list_.Set(creature::HEALTH, Stat(result->getUInt("current_health")));
                creature->stat_current_list_.Set(creature::STRENGTH, Stat(result->getUInt("current_strength")));
                creature->stat_current_list_.Set(creature::CONSTITUTION, Stat(result->getUInt("current_constitution")));
                creature->stat_current_list_.Set(creature::ACTION, Stat(result->getUInt("current_action")));
                creature->stat_current_list_.Set(creature::QUICKNESS, Stat(result->getUInt("current_quickness")));
                creature->stat_current_list_.Set(creature::STAMINA, Stat(result->getUInt("current_stamina")));
                creature->stat_current_list_.Set(creature::MIND, Stat(result->getUInt("current_mind")));
                creature->stat_current_list_.Set(creature::FOCUS, Stat(result->getUInt("current_focus")));
                creature->stat_current_list_.Set(creature::WILLPOWER, Stat(result->getUInt("current_willpower")));

                creature->stat_max_list_.Set(creature::HEALTH, Stat(result->getUInt("max_health")));
                creature->stat_max_list_.Set(creature::STRENGTH, Stat(result->getUInt("max_strength")));
                creature->stat_max_list_.Set(creature::CONSTITUTION, Stat(result->getUInt("max_constitution")));
                creature->stat_max_list_.Set(creature::ACTION, Stat(result->getUInt("max_action")));
                creature->stat_max_list_.Set(creature::QUICKNESS, Stat(result->getUInt("max_quickness")));
                creature->stat_max_list_.Set(creature::STAMINA, Stat(result->getUInt("max_stamina")));
                creature->stat_max_list_.Set(creature::MIND, Stat(result->getUInt("max_mind")));
                creature->stat_max_list_.Set(creature::FOCUS, Stat(result->getUInt("max_focus")));
                creature->stat_max_list_.Set(creature::WILLPOWER, Stat(result->getUInt("max_willpower")));

                creature->stat_wound_list_.Set(creature::HEALTH, Stat(result->getUInt("health_wounds")));
                creature->stat_wound_list_.Set(creature::STRENGTH, Stat(result->getUInt("strength_wounds")));
                creature->stat_wound_list_.Set(creature::CONSTITUTION, Stat(result->getUInt("constitution_wounds")));
                creature->stat_wound_list_.Set(creature::ACTION, Stat(result->getUInt("action_wounds")));
                creature->stat_wound_list_.Set(creature::QUICKNESS, Stat(result->getUInt("quickness_wounds")));
                creature->stat_wound_list_.Set(creature::STAMINA, Stat(result->getUInt("stamina_wounds")));
                creature->stat_wound_list_.Set(creature::MIND, Stat(result->getUInt("mind_wounds")));
                creature->stat_wound_list_.Set(creature::FOCUS, Stat(result->getUInt("focus_wounds")));
                creature->stat_wound_list_.Set(creature::WILLPOWER, Stat(result->getUInt("willpower_wounds")));

                creature->stat_base_list_.Set(creature::HEALTH, Stat(result->getUInt("health_wounds")));
                creature->stat_base_list_.Set(creature::STRENGTH, Stat(result->getUInt("strength_wounds")));
                creature->stat_base_list_.Set(creature::CONSTITUTION, Stat(result->getUInt("constitution_wounds")));
                creature->stat_base_list_.Set(creature::ACTION, Stat(result->getUInt("action_wounds")));
                creature->stat_base_list_.Set(creature::QUICKNESS, Stat(result->getUInt("quickness_wounds")));
                creature->stat_base_list_.Set(creature::STAMINA, Stat(result->getUInt("stamina_wounds")));
                creature->stat_base_list_.Set(creature::MIND, Stat(result->getUInt("mind_wounds")));
                creature->stat_base_list_.Set(creature::FOCUS, Stat(result->getUInt("focus_wounds")));
                creature->stat_base_list_.Set(creature::WILLPOWER, Stat(result->getUInt("willpower_wounds")));
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

            creature->skills_.Insert(Skill(skill_name));
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

            creature->skill_mod_list_.Insert(
                skill_mod_name, 
                SkillMod(skill_mod_name, skill_mod_value, 0));
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