
#include "swganh/object/creature/creature_factory.h"

#include <sstream>

#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/creature/creature.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::creature;
using namespace swganh::simulation;


CreatureFactory::CreatureFactory(const shared_ptr<DatabaseManagerInterface>& db_manager,
                             SimulationService* simulation_service)
    : TangibleFactory(db_manager, simulation_service)
{
}

void CreatureFactory::LoadTemplates()
{}

bool CreatureFactory::HasTemplate(const string& template_name)
{
    return false;
}

void CreatureFactory::PersistObject(const shared_ptr<Object>& object)
{}

void CreatureFactory::DeleteObjectFromStorage(const shared_ptr<Object>& object)
{}

shared_ptr<Object> CreatureFactory::CreateObjectFromStorage(uint64_t object_id)
{
    auto creature = make_shared<Creature>();
    creature->SetObjectId(object_id);
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = shared_ptr<sql::Statement>(conn->createStatement());
        shared_ptr<sql::ResultSet> result;
        stringstream ss;
        ss << "CALL sp_GetCreature(" << object_id << ");" ;
        statement->execute(ss.str());
        CreateBaseTangible(creature, statement);
        
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
                creature->scale_ = result->getDouble("scale");
                creature->battle_fatigue_ = result->getUInt("battle_fatigue");
                creature->state_bitmask_ = result->getUInt("state");
                creature->acceleration_multiplier_base_ = result->getDouble("acceleration_base");
                creature->acceleration_multiplier_modifier_ = result->getDouble("acceleration_modifier");
                creature->speed_multiplier_base_ = result->getDouble("speed_base");
                creature->speed_multiplier_modifier_ = result->getDouble("speed_modifier");
                creature->run_speed_ = result->getDouble("run_speed");
                creature->slope_modifier_angle_ = result->getDouble("slope_modifier_angle");
                creature->slope_modifier_percent_ = result->getDouble("slope_modifier_percent");
                creature->walking_speed_ = result->getDouble("walking_speed");
                creature->turn_radius_ = result->getDouble("turn_radius");
                creature->water_modifier_percent_ = result->getDouble("water_modifier_percent");
                creature->combat_level_ = result->getUInt("combat_level");
                creature->animation_ = result->getString("animation");
                creature->mood_animation_ = result->getString("mood_animation");
                creature->group_id_ = result->getUInt64("group_id");
                creature->guild_id_ = result->getUInt("guild_id");
                creature->weapon_id_ = result->getUInt64("weapon_id");
                creature->mood_id_ = result->getUInt("mood_id");
                creature->performance_id_ = result->getUInt("performance_id");
                creature->disguise_ = result->getString("iff_template");

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

        // Check for contained objects        
        if (statement->getMoreResults())
        {
            result.reset(statement->getResultSet());

            uint64_t contained_id;
            uint32_t contained_type;
            while (result->next())
            {
                contained_id = result->getUInt64("id");
                contained_type = result->getUInt("type_id");
                auto object = simulation_service_->LoadObjectById(contained_id, contained_type);

                creature->AddContainedObject(object, Object::LINK);
            }
        }
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    return creature;
}

shared_ptr<Object> CreatureFactory::CreateObjectFromTemplate(const string& template_name)
{
    return make_shared<Creature>();
}
