
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

using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::object::creature;


CreatureFactory::CreatureFactory(const shared_ptr<DatabaseManagerInterface>& db_manager)
    : TangibleFactory(db_manager)
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
                creature->SetOwnerId(result->getUInt64("owner_id"));
                creature->SetBankCredits(result->getUInt("bank_credits"));
                creature->SetCashCredits(result->getUInt("cash_credits"));
                creature->SetPosture((Posture)result->getUInt("posture"));
                creature->SetFactionRank(result->getUInt("faction_rank"));
                creature->SetScale(result->getDouble("scale"));
                creature->SetBattleFatigue(result->getUInt("battle_fatigue"));
                creature->SetStateBitmask(result->getUInt("state"));
                // TODO: Add rest of creature table
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
