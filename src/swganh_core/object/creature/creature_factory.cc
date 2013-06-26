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
#include "swganh_core/simulation/simulation_service_interface.h"

using namespace std;
using namespace swganh::database;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::simulation;


CreatureFactory::CreatureFactory(swganh::app::SwganhKernel* kernel)
    : TangibleFactory(kernel)
{}

void CreatureFactory::LoadFromStorage(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock)
{
    TangibleFactory::LoadFromStorage(connection, object, lock);

    auto creature = std::dynamic_pointer_cast<Creature>(object);
    if(!creature)
    {
        throw InvalidObject("Object requested for loading is not Intangible");
    }

    auto statement = std::unique_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetCreature(?);"));
    
    statement->setUInt64(1, creature->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            creature->SetOwnerId(result->getUInt64("owner_id"), lock);
            creature->SetListenToId(result->getUInt64("musician_id"), lock);
            creature->SetBankCredits(result->getUInt("bank_credits"), lock);
            creature->SetCashCredits(result->getUInt("cash_credits"), lock);
            creature->SetPosture((Posture)result->getUInt("posture"), lock);
            creature->SetFactionRank(result->getUInt("faction_rank"), lock);
            creature->SetScale(static_cast<float>(result->getDouble("scale")), lock);
            creature->SetBattleFatigue(result->getUInt("battle_fatigue"), lock);
            creature->SetStateBitmask(result->getUInt("state"), lock);
            creature->SetAccelerationMultiplierBase(static_cast<float>(result->getDouble("acceleration_base")), lock);
            creature->SetAccelerationMultiplierModifier(static_cast<float>(result->getDouble("acceleration_modifier")), lock);
            creature->SetSpeedMultiplierBase(static_cast<float>(result->getDouble("speed_base")), lock);
            creature->SetSpeedMultiplierModifier(static_cast<float>(result->getDouble("speed_modifier")), lock);
            creature->SetRunSpeed(static_cast<float>(result->getDouble("run_speed")), lock);
            creature->SetSlopeModifierAngle(static_cast<float>(result->getDouble("slope_modifier_angle")), lock);
            creature->SetSlopeModifierPercent(static_cast<float>(result->getDouble("slope_modifier_percent")), lock);
            creature->SetWalkingSpeed(static_cast<float>(result->getDouble("walking_speed")), lock);
            creature->SetTurnRadius(static_cast<float>(result->getDouble("turn_radius")), lock);
            creature->SetWaterModifierPercent(static_cast<float>(result->getDouble("water_modifier_percent")), lock);
            creature->SetCombatLevel(result->getUInt("combat_level"), lock);
            creature->SetAnimation(result->getString("animation"), lock);
            creature->SetMoodAnimation(result->getString("mood_animation"), lock);
            
            /// @TODO: Find a better place for this.
            if (creature->GetMoodAnimation(lock).compare("none") == 0)
            {
                creature->SetMoodAnimation("neutral", lock);
            }

            creature->SetGroupId(result->getUInt64("group_id"), lock);
            creature->SetGuildId(result->getUInt("guild_id"), lock);
            creature->SetWeaponId(result->getUInt64("weapon_id"), lock);
            creature->SetMoodId(result->getUInt("mood_id"), lock);
            creature->SetPerformanceId(result->getUInt("performance_id"), lock);
            creature->SetDisguise(result->getString("disguise_template"), lock);
            
            creature->SetStatCurrent(HEALTH, result->getUInt("current_health"), lock);
            creature->SetStatCurrent(STRENGTH, result->getUInt("current_strength"), lock);
            creature->SetStatCurrent(CONSTITUTION, result->getUInt("current_constitution"), lock);
            creature->SetStatCurrent(ACTION, result->getUInt("current_action"), lock);
            creature->SetStatCurrent(QUICKNESS, result->getUInt("current_quickness"), lock);
            creature->SetStatCurrent(STAMINA, result->getUInt("current_stamina"), lock);
            creature->SetStatCurrent(MIND, result->getUInt("current_mind"), lock);
            creature->SetStatCurrent(FOCUS, result->getUInt("current_focus"), lock);
            creature->SetStatCurrent(WILLPOWER, result->getUInt("current_willpower"), lock);
            
            creature->SetStatMax(HEALTH, result->getUInt("max_health"), lock);
            creature->SetStatMax(STRENGTH, result->getUInt("max_strength"), lock);
            creature->SetStatMax(CONSTITUTION, result->getUInt("max_constitution"), lock);
            creature->SetStatMax(ACTION, result->getUInt("max_action"), lock);
            creature->SetStatMax(QUICKNESS, result->getUInt("max_quickness"), lock);
            creature->SetStatMax(STAMINA, result->getUInt("max_stamina"), lock);
            creature->SetStatMax(MIND, result->getUInt("max_mind"), lock);
            creature->SetStatMax(FOCUS, result->getUInt("max_focus"), lock);
            creature->SetStatMax(WILLPOWER, result->getUInt("max_willpower"), lock);
            
            creature->SetStatWound(HEALTH, result->getUInt("health_wounds"), lock);
            creature->SetStatWound(STRENGTH, result->getUInt("strength_wounds"), lock);
            creature->SetStatWound(CONSTITUTION, result->getUInt("constitution_wounds"), lock);
            creature->SetStatWound(ACTION, result->getUInt("action_wounds"), lock);
            creature->SetStatWound(QUICKNESS, result->getUInt("quickness_wounds"), lock);
            creature->SetStatWound(STAMINA, result->getUInt("stamina_wounds"), lock);
            creature->SetStatWound(MIND, result->getUInt("mind_wounds"), lock);
            creature->SetStatWound(FOCUS, result->getUInt("focus_wounds"), lock);
            creature->SetStatWound(WILLPOWER, result->getUInt("willpower_wounds"), lock);
            
            creature->SetStatBase(HEALTH, result->getUInt("health_wounds"), lock);
            creature->SetStatBase(STRENGTH, result->getUInt("strength_wounds"), lock);
            creature->SetStatBase(CONSTITUTION, result->getUInt("constitution_wounds"), lock);
            creature->SetStatBase(ACTION, result->getUInt("action_wounds"), lock);
            creature->SetStatBase(QUICKNESS, result->getUInt("quickness_wounds"), lock);
            creature->SetStatBase(STAMINA, result->getUInt("stamina_wounds"), lock);
            creature->SetStatBase(MIND, result->getUInt("mind_wounds"), lock);
            creature->SetStatBase(FOCUS, result->getUInt("focus_wounds"), lock);
            creature->SetStatBase(WILLPOWER, result->getUInt("willpower_wounds"), lock);
        }
    } while(statement->getMoreResults());

    LoadBuffs_(connection, creature, lock);
    LoadSkills_(connection, creature, lock);
    LoadSkillMods_(connection, creature, lock);
    LoadSkillCommands_(connection, creature, lock);
}

void CreatureFactory::RegisterEventHandlers()
{
	GetEventDispatcher()->Subscribe("Creature::Bank", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::Cash", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::StatBase", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::Skill", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::Posture", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::OwnerId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::Scale", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::BattleFatigue", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::StateBitmask", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::StatWound", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::AccelerationMultiplierBase", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::AccelerationMultiplierModifier", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::StatEncumberance", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::SkillMod", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::SpeedMultiplierBase", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::SpeedMultiplierModifer", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::ListenToId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::RunSpeed", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::SlopeModifierAngle", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::SlopeModifierPercent", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::TurnRadius", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::WalkingSpeed", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::WaterModifierPercent", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::MissionCriticalObject", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::CombatLevel", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::Animation", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::WeaponId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::GroupId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::InviteSenderId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::GuildId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::TargetId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::MoodId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::PerformanceId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::StatCurrent", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::StatMax", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::EquipmentItem", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::Disguise", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::Stationary", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	GetEventDispatcher()->Subscribe("Creature::PvPStatus", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
}
void CreatureFactory::PersistChangedObjects()
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
uint32_t CreatureFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
    uint32_t counter = 1;

	TangibleFactory::PersistObject(object, lock, persist_inherited);

	// Now for the biggy
    try
    {	
        auto conn = GetDatabaseManager()->getConnection("galaxy");
        // 65 of these
        string sql = "CALL sp_PersistCreature(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,"
            "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);";
        auto statement = unique_ptr<sql::PreparedStatement>(conn->prepareStatement(sql));
        auto creature = static_pointer_cast<Creature>(object);
        statement->setUInt64(counter++, creature->GetObjectId(lock));
        statement->setUInt64(counter++, creature->GetOwnerId(lock));
        statement->setUInt64(counter++, creature->GetListenToId(lock));
        statement->setUInt64(counter++, creature->GetBankCredits(lock));
        statement->setUInt64(counter++, creature->GetCashCredits(lock));
        statement->setUInt64(counter++, creature->GetPosture(lock));
        statement->setUInt(counter++, creature->GetFactionRank(lock));
        statement->setDouble(counter++, creature->GetScale(lock));
        statement->setUInt64(counter++, creature->GetBattleFatigue(lock));
        statement->setUInt64(counter++, creature->GetStateBitmask(lock));
        statement->setDouble(counter++, creature->GetAccelerationMultiplierBase(lock));
        statement->setDouble(counter++, creature->GetAccelerationMultiplierModifier(lock));
        statement->setDouble(counter++, creature->GetSpeedMultiplierBase(lock));
        statement->setDouble(counter++, creature->GetSpeedMultiplierModifier(lock));
        statement->setDouble(counter++, creature->GetRunSpeed(lock));
        statement->setDouble(counter++, creature->GetSlopeModifierAngle(lock));
        statement->setDouble(counter++, creature->GetSlopeModifierPercent(lock));
        statement->setDouble(counter++, creature->GetTurnRadius(lock));
        statement->setDouble(counter++, creature->GetWalkingSpeed(lock));
        statement->setDouble(counter++, creature->GetWaterModifierPercent(lock));
        statement->setInt(counter++, creature->GetCombatLevel(lock));
        statement->setString(counter++, creature->GetAnimation(lock));
        statement->setUInt64(counter++, creature->GetGroupId(lock));
        statement->setUInt(counter++, creature->GetGuildId(lock));
        statement->setUInt64(counter++, creature->GetWeaponId(lock));
        statement->setUInt(counter++, creature->GetMoodId(lock));
        statement->setUInt(counter++, creature->GetPerformanceId(lock));
        statement->setString(counter++, creature->GetDisguise(lock));
        // WOUNDS
        statement->setInt(counter++, creature->GetStatWound(HEALTH, lock));
        statement->setInt(counter++, creature->GetStatWound(STRENGTH, lock));
        statement->setInt(counter++, creature->GetStatWound(CONSTITUTION, lock));
        statement->setInt(counter++, creature->GetStatWound(ACTION, lock));
        statement->setInt(counter++, creature->GetStatWound(QUICKNESS, lock));
        statement->setInt(counter++, creature->GetStatWound(STAMINA, lock));
        statement->setInt(counter++, creature->GetStatWound(MIND, lock));
        statement->setInt(counter++, creature->GetStatWound(FOCUS, lock));
        statement->setInt(counter++, creature->GetStatWound(WILLPOWER, lock));
        // ENCUMBERANCE
        statement->setInt(counter++, creature->GetStatEncumberance(HEALTH, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(STRENGTH, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(CONSTITUTION, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(ACTION, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(QUICKNESS, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(STAMINA, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(MIND, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(FOCUS, lock));
        statement->setInt(counter++, creature->GetStatEncumberance(WILLPOWER, lock));
        // CURRENT
        statement->setInt(counter++, creature->GetStatCurrent(HEALTH, lock));
        statement->setInt(counter++, creature->GetStatCurrent(STRENGTH, lock));
        statement->setInt(counter++, creature->GetStatCurrent(CONSTITUTION, lock));
        statement->setInt(counter++, creature->GetStatCurrent(ACTION, lock));
        statement->setInt(counter++, creature->GetStatCurrent(QUICKNESS, lock));
        statement->setInt(counter++, creature->GetStatCurrent(STAMINA, lock));
        statement->setInt(counter++, creature->GetStatCurrent(MIND, lock));
        statement->setInt(counter++, creature->GetStatCurrent(FOCUS, lock));
        statement->setInt(counter++, creature->GetStatCurrent(WILLPOWER, lock));
        // MAX
        statement->setInt(counter++, creature->GetStatMax(HEALTH, lock));
        statement->setInt(counter++, creature->GetStatMax(STRENGTH, lock));
        statement->setInt(counter++, creature->GetStatMax(CONSTITUTION, lock));
        statement->setInt(counter++, creature->GetStatMax(ACTION, lock));
        statement->setInt(counter++, creature->GetStatMax(QUICKNESS, lock));
        statement->setInt(counter++, creature->GetStatMax(STAMINA, lock));
        statement->setInt(counter++, creature->GetStatMax(MIND, lock));
        statement->setInt(counter++, creature->GetStatMax(FOCUS, lock));
        statement->setInt(counter++, creature->GetStatMax(WILLPOWER, lock));

        int updated = statement->executeUpdate();
				
        LOG(warning) << "Updated " << updated << " rows in sp_PersistCreature with object id = " << object->GetObjectId(lock);
    }
    catch(sql::SQLException &e)
    {
        LOG(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        LOG(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
	return counter;
}

void CreatureFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{
	ObjectFactory::DeleteObjectFromStorage(object);
}

shared_ptr<Object> CreatureFactory::CreateObject()
{
	return make_shared<Creature>();
}

void CreatureFactory::LoadSkills_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Creature>& creature, 
								  boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetCreatureSkills(?);"));
    
    statement->setUInt64(1, creature->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());    
    do
    {
        while (result->next())
        {
            creature->AddSkill(result->getString("name"), lock);
        }
    } while(statement->getMoreResults());
}

void CreatureFactory::LoadSkillMods_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Creature>& creature, 
									 boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetCreatureSkillMods(?);"));
    
    statement->setUInt64(1, creature->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());    
    do
    {
        while (result->next())
        {
            creature->AddSkillMod(SkillMod(result->getString("name"), result->getUInt("value"), 0), lock);	
        }
    } while(statement->getMoreResults());
}

void CreatureFactory::LoadSkillCommands_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<Creature>& creature, 
										 boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetCreatureSkillCommands(?);"));
    
    statement->setUInt64(1, creature->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {  
        while (result->next())
        {
            creature->AddSkillCommand(std::make_pair(result->getInt("id"), result->getString("name")), lock);
        }
    } while(statement->getMoreResults());
}

void CreatureFactory::LoadBuffs_(const std::shared_ptr<sql::Connection>& connection, const std::shared_ptr<swganh::object::Creature>& creature, 
								 boost::unique_lock<boost::mutex>& lock)
{
    auto statement = std::shared_ptr<sql::PreparedStatement>
        (connection->prepareStatement("CALL sp_GetCreatureBuffs(?);"));
    
    statement->setUInt64(1, creature->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
            creature->AddBuff(result->getString("name"), result->getUInt("duration"));
        }
    } while(statement->getMoreResults());
}
