// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "creature_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include "swganh/logger.h"

#include "swganh/database/database_manager.h"
#include "swganh_core/object/creature/creature.h"
#include "swganh_core/object/exception.h"
#include "swganh/simulation/simulation_service_interface.h"

using namespace std;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;


CreatureFactory::CreatureFactory(DatabaseManagerInterface* db_manager,
                             swganh::EventDispatcher* event_dispatcher)
    : TangibleFactory(db_manager, event_dispatcher)
{
}

uint32_t CreatureFactory::PersistObject(const shared_ptr<Object>& object)
{
    // Persist Tangible and Base Object First
    uint32_t counter = 1;
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
        statement->setUInt64(counter++, creature->GetObjectId());
        statement->setUInt64(counter++, creature->GetOwnerId());
        statement->setUInt64(counter++, creature->GetListenToId());
        statement->setUInt64(counter++, creature->GetBankCredits());
        statement->setUInt64(counter++, creature->GetCashCredits());
        statement->setUInt64(counter++, creature->GetPosture());
        statement->setUInt(counter++, creature->GetFactionRank());
        statement->setDouble(counter++, creature->GetScale());
        statement->setUInt64(counter++, creature->GetBattleFatigue());
        statement->setUInt64(counter++, creature->GetStateBitmask());
        statement->setDouble(counter++, creature->GetAccelerationMultiplierBase());
        statement->setDouble(counter++, creature->GetAccelerationMultiplierModifier());
        statement->setDouble(counter++, creature->GetSpeedMultiplierBase());
        statement->setDouble(counter++, creature->GetSpeedMultiplierModifier());
        statement->setDouble(counter++, creature->GetRunSpeed());
        statement->setDouble(counter++, creature->GetSlopeModifierAngle());
        statement->setDouble(counter++, creature->GetSlopeModifierPercent());
        statement->setDouble(counter++, creature->GetTurnRadius());
        statement->setDouble(counter++, creature->GetWalkingSpeed());
        statement->setDouble(counter++, creature->GetWaterModifierPercent());
        statement->setInt(counter++, creature->GetCombatLevel());
        statement->setString(counter++, creature->GetAnimation());
        statement->setUInt64(counter++, creature->GetGroupId());
        statement->setUInt(counter++, creature->GetGuildId());
        statement->setUInt64(counter++, creature->GetWeaponId());
        statement->setUInt(counter++, creature->GetMoodId());
        statement->setUInt(counter++, creature->GetPerformanceId());
        statement->setString(counter++, creature->GetDisguise());
        // WOUNDS
        statement->setInt(counter++, creature->GetStatWound(HEALTH));
        statement->setInt(counter++, creature->GetStatWound(STRENGTH));
        statement->setInt(counter++, creature->GetStatWound(CONSTITUTION));
        statement->setInt(counter++, creature->GetStatWound(ACTION));
        statement->setInt(counter++, creature->GetStatWound(QUICKNESS));
        statement->setInt(counter++, creature->GetStatWound(STAMINA));
        statement->setInt(counter++, creature->GetStatWound(MIND));
        statement->setInt(counter++, creature->GetStatWound(FOCUS));
        statement->setInt(counter++, creature->GetStatWound(WILLPOWER));
        // ENCUMBERANCE
        statement->setInt(counter++, creature->GetStatEncumberance(HEALTH));
        statement->setInt(counter++, creature->GetStatEncumberance(STRENGTH));
        statement->setInt(counter++, creature->GetStatEncumberance(CONSTITUTION));
        statement->setInt(counter++, creature->GetStatEncumberance(ACTION));
        statement->setInt(counter++, creature->GetStatEncumberance(QUICKNESS));
        statement->setInt(counter++, creature->GetStatEncumberance(STAMINA));
        statement->setInt(counter++, creature->GetStatEncumberance(MIND));
        statement->setInt(counter++, creature->GetStatEncumberance(FOCUS));
        statement->setInt(counter++, creature->GetStatEncumberance(WILLPOWER));
        // CURRENT
        statement->setInt(counter++, creature->GetStatCurrent(HEALTH));
        statement->setInt(counter++, creature->GetStatCurrent(STRENGTH));
        statement->setInt(counter++, creature->GetStatCurrent(CONSTITUTION));
        statement->setInt(counter++, creature->GetStatCurrent(ACTION));
        statement->setInt(counter++, creature->GetStatCurrent(QUICKNESS));
        statement->setInt(counter++, creature->GetStatCurrent(STAMINA));
        statement->setInt(counter++, creature->GetStatCurrent(MIND));
        statement->setInt(counter++, creature->GetStatCurrent(FOCUS));
        statement->setInt(counter++, creature->GetStatCurrent(WILLPOWER));
        // MAX
        statement->setInt(counter++, creature->GetStatMax(HEALTH));
        statement->setInt(counter++, creature->GetStatMax(STRENGTH));
        statement->setInt(counter++, creature->GetStatMax(CONSTITUTION));
        statement->setInt(counter++, creature->GetStatMax(ACTION));
        statement->setInt(counter++, creature->GetStatMax(QUICKNESS));
        statement->setInt(counter++, creature->GetStatMax(STAMINA));
        statement->setInt(counter++, creature->GetStatMax(MIND));
        statement->setInt(counter++, creature->GetStatMax(FOCUS));
        statement->setInt(counter++, creature->GetStatMax(WILLPOWER));

        int updated = statement->executeUpdate();
				
        LOG(warning) << "Updated " << updated << " rows in sp_PersistCreature";
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
	return counter;
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

shared_ptr<Object> CreatureFactory::CreateObjectFromTemplate(const string& template_name, bool db_persisted, bool db_initialized)
{
	if(db_persisted || db_initialized)
	{
		//TODO: Have to hit the db to make this
		return make_shared<Creature>();
	}
	else
	{
		return make_shared<Creature>();
	}
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