
#include "swganh/object/creature/creature_factory.h"

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

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::creature;


CreatureFactory::CreatureFactory(const shared_ptr<DatabaseManagerInterface>& db_manager)
    : ObjectFactory(db_manager)
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
    CreateBaseObjectFromStorage(creature);
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetCreature(?);");
        statement->setUInt64(1, object_id);
        auto result = shared_ptr<sql::ResultSet>(statement->executeQuery());
        while (result->next())
        {
            creature->SetOwnerId(result->getUInt64("owner_id"));
            creature->SetBankCredits(result->getUInt("bank_credits"));
            creature->SetCashCredits(result->getUInt("cash_credits"));
            creature->SetPosture((Creature::POSTURE)result->getUInt("posture"));
            creature->SetFactionRank(result->getUInt("faction_rank"));
            creature->SetScale(result->getDouble("scale"));
            creature->SetBattleFatigue(result->getUInt("battle_fatigue"));
            creature->SetStateBitmask(result->getUInt("state"));
            // TODO: Add rest of creature table
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
