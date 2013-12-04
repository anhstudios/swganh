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
        throw InvalidObject("Object requested for loading is not a creature");
    }

    auto statement = std::unique_ptr<sql::PreparedStatement>
                     (connection->prepareStatement("CALL sp_GetCreature(?);"));

    statement->setUInt64(1, creature->GetObjectId(lock));

    auto result = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
    do
    {
        while (result->next())
        {
			//why the heck would you want to persist these two values ????
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
            creature->SetTurnRadius(static_cast<float>(result->getDouble("turn_radius")), lock);
			creature->SetWalkingSpeed(static_cast<float>(result->getDouble("walking_speed")), lock);
            creature->SetWaterModifierPercent(static_cast<float>(result->getDouble("water_modifier_percent")), lock);
            creature->SetCombatLevel(result->getUInt("combat_level"), lock);

			//why the heck would animation be persisted ?
            creature->SetAnimation(result->getString("animation"), lock);
            
			
			creature->SetMoodAnimation(result->getString("mood_animation"), lock);

            /// @TODO: Find a better place for this.
            if (creature->GetMoodAnimation(lock).compare("none") == 0)
            {
                creature->SetMoodAnimation("neutral", lock);
            }

			//why persist groupId??????
            creature->SetGroupId(result->getUInt64("group_id"), lock);
            
			creature->SetGuildId(result->getUInt("guild_id"), lock);
            creature->SetWeaponId(result->getUInt64("weapon_id"), lock);
            creature->SetMoodId(result->getUInt("mood_id"), lock);

			//why the heck would performance_id be persisted ?
            creature->SetPerformanceId(result->getUInt("performance_id"), lock);
            
			creature->SetDisguise(result->getString("disguise_template"), lock);

			creature->SetStatWound(HEALTH, result->getUInt("health_wounds"), lock, false);
            creature->SetStatWound(STRENGTH, result->getUInt("strength_wounds"), lock, false);
            creature->SetStatWound(CONSTITUTION, result->getUInt("constitution_wounds"), lock, false);
            creature->SetStatWound(ACTION, result->getUInt("action_wounds"), lock, false);
            creature->SetStatWound(QUICKNESS, result->getUInt("quickness_wounds"), lock, false);
            creature->SetStatWound(STAMINA, result->getUInt("stamina_wounds"), lock, false);
            creature->SetStatWound(MIND, result->getUInt("mind_wounds"), lock, false);
            creature->SetStatWound(FOCUS, result->getUInt("focus_wounds"), lock, false);
            creature->SetStatWound(WILLPOWER, result->getUInt("willpower_wounds"), lock, false);
			
			creature->SetStatCurrent(HEALTH, result->getUInt("current_health"), lock, false);
            creature->SetStatCurrent(STRENGTH, result->getUInt("current_strength"), lock, false);
            creature->SetStatCurrent(CONSTITUTION, result->getUInt("current_constitution"), lock, false);
            creature->SetStatCurrent(ACTION, result->getUInt("current_action"), lock, false);
            creature->SetStatCurrent(QUICKNESS, result->getUInt("current_quickness"), lock, false);
            creature->SetStatCurrent(STAMINA, result->getUInt("current_stamina"), lock, false);
            creature->SetStatCurrent(MIND, result->getUInt("current_mind"), lock, false);
            creature->SetStatCurrent(FOCUS, result->getUInt("current_focus"), lock, false);
            creature->SetStatCurrent(WILLPOWER, result->getUInt("current_willpower"), lock, false);

            creature->SetStatMax(HEALTH, result->getUInt("max_health"), lock, false);
            creature->SetStatMax(STRENGTH, result->getUInt("max_strength"), lock, false);
            creature->SetStatMax(CONSTITUTION, result->getUInt("max_constitution"), lock, false);
            creature->SetStatMax(ACTION, result->getUInt("max_action"), lock, false);
            creature->SetStatMax(QUICKNESS, result->getUInt("max_quickness"), lock, false);
            creature->SetStatMax(STAMINA, result->getUInt("max_stamina"), lock, false);
            creature->SetStatMax(MIND, result->getUInt("max_mind"), lock, false);
            creature->SetStatMax(FOCUS, result->getUInt("max_focus"), lock, false);
            creature->SetStatMax(WILLPOWER, result->getUInt("max_willpower"), lock, false);

			creature->SetStatEncumberance(HEALTH, result->getUInt("health_encumberance"), lock, false);
			creature->SetStatEncumberance(STRENGTH, result->getUInt("strength_encumberance"), lock, false);
            creature->SetStatEncumberance(CONSTITUTION, result->getUInt("constitution_encumberance"), lock, false);
            creature->SetStatEncumberance(ACTION, result->getUInt("action_encumberance"), lock, false);
            creature->SetStatEncumberance(QUICKNESS, result->getUInt("quickness_encumberance"), lock, false);
            creature->SetStatEncumberance(STAMINA, result->getUInt("stamina_encumberance"), lock, false);
            creature->SetStatEncumberance(MIND, result->getUInt("mind_encumberance"), lock, false);
            creature->SetStatEncumberance(FOCUS, result->getUInt("focus_encumberance"), lock, false);
            creature->SetStatEncumberance(WILLPOWER, result->getUInt("willpower_encumberance"), lock, false);

            creature->SetStatBase(HEALTH, result->getUInt("current_health"), lock);
            creature->SetStatBase(STRENGTH, result->getUInt("current_strength"), lock);
            creature->SetStatBase(CONSTITUTION, result->getUInt("current_constitution"), lock);
            creature->SetStatBase(ACTION, result->getUInt("current_action"), lock);
            creature->SetStatBase(QUICKNESS, result->getUInt("current_quickness"), lock);
            creature->SetStatBase(STAMINA, result->getUInt("current_stamina"), lock);
            creature->SetStatBase(MIND, result->getUInt("current_mind"), lock);
            creature->SetStatBase(FOCUS, result->getUInt("current_focus"), lock);
            creature->SetStatBase(WILLPOWER, result->getUInt("current_willpower"), lock);
        }
    }
    while(statement->getMoreResults());

    LoadBuffs_(connection, creature, lock);
    LoadSkills_(connection, creature, lock);
    LoadSkillMods_(connection, creature, lock);
}

void CreatureFactory::RegisterEventHandlers()
{
    GetEventDispatcher()->Subscribe("Creature::Bank", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::Cash", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::StatBase", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    
	GetEventDispatcher()->Subscribe("Creature::Skill", std::bind(&CreatureFactory::PersistHandlerQueue, this, std::placeholders::_1));
    
	GetEventDispatcher()->Subscribe("Creature::Posture", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::OwnerId", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::Scale", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::BattleFatigue", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::StateBitmask", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::StatWound", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::AccelerationMultiplierBase", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::AccelerationMultiplierModifier", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    GetEventDispatcher()->Subscribe("Creature::StatEncumberance", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    
	//nothing really to persist here
	//GetEventDispatcher()->Subscribe("Creature::SkillMod", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
	//GetEventDispatcher()->Subscribe("Creature::SkillCommand", std::bind(&CreatureFactory::PersistHandler, this, std::placeholders::_1));
    
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



//ok what exactly do you want to achieve here ???
//you get the iterator for a persistent object (out of a set no less (for what reason?))
//with a trigger that gets lost after the object has been added
//so we dont even know what we want to persist ...

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

	//the advantage of the set is actualle that the respective Object can be in there only once
	//without us having to actively check if its in there
	//the queue on the other hand has much less overhead is faster (in every respect as the amount of data 
	//will usually be small and we dont need log n as guaranteed time)
	//but we might end up persisiting one object several times without the actual need
	//I dont really think the issue warrants a dirty flag just yet or writing a sort handler for a set pair
	std::pair<shared_ptr<Object>,HashString> p1;
	while(!persisted_objects_queue_.empty())
    {
		p1 = persisted_objects_queue_.front();
		persisted_objects_queue_.pop();
		auto object = p1.first;
        auto lock = object->AcquireLock();
        if(object->IsDatabasePersisted(lock))
        {
			if (p1.second.ident_string() == "Creature::Skill")
			{
				PersistSkills(object,lock);
			}
        }
    }

}

uint32_t CreatureFactory::PersistSkills(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
	uint32_t counter = 0;

	auto conn		= GetDatabaseManager()->getConnection("galaxy");
	auto creature	= static_pointer_cast<Creature>(object);
    auto skills		= creature->GetSkillsSyncQueue(lock);

        while(skills.size())
        {
            auto queue_item = skills.front();

            switch(queue_item.first)
            {
            case 0: // Remove
            {
                auto statement = conn->prepareStatement("CALL sp_RemoveSkill(?, ?);");
                statement->setUInt64(1, creature->GetObjectId(lock));
				statement->setString(2, queue_item.second);
                auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
                break;
            }

            case 1: // Add
            {
                auto statement = conn->prepareStatement("CALL sp_PersistSkill(?, ?);");
                statement->setUInt64(1, creature->GetObjectId(lock));
                statement->setString(2, queue_item.second);
                auto result = unique_ptr<sql::ResultSet>(statement->executeQuery());
                break;
            }
            }

            skills.pop();
        }

	
	return counter;
}

uint32_t CreatureFactory::PersistObject(const shared_ptr<Object>& object, boost::unique_lock<boost::mutex>& lock, bool persist_inherited)
{
    uint32_t counter = 1;

    TangibleFactory::PersistObject(object, lock, persist_inherited);



    // Now for the biggy
    try
    {

		//auto skill = static_pointer_cast<reature>(object);

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

		//save the skills to the db
		//PersistSkills(creature,lock);
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
            creature->AddSkill(result->getString("name"), lock, false);
        }
    }
    while(statement->getMoreResults());
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
            creature->AddSkillMod(SkillMod(result->getString("name"), result->getUInt("value"), 0), lock, false);
        }
    }
    while(statement->getMoreResults());
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
    }
    while(statement->getMoreResults());
}
