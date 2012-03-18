#include "swganh/social/social_service.h"

#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>

#include <boost/log/trivial.hpp>

#include "anh/service/service_manager.h"
#include "anh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/connection/connection_service.h"

#include "swganh/object/player/player.h"
#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/object_manager.h"

#include "swganh/messages/out_of_band.h"

using namespace anh::database;
using namespace std;
using namespace swganh::connection;
using namespace swganh::object;
using namespace swganh::object::player;
using namespace swganh::social;

using anh::app::KernelInterface;
using anh::service::ServiceDescription;
using swganh::base::BaseService;

SocialService::SocialService(KernelInterface* kernel)
    : BaseService(kernel)
{}

SocialService::~SocialService()
{}

ServiceDescription SocialService::GetServiceDescription()
{
    ServiceDescription service_description(
        "SocialService",
        "social",
        "0.1",
        "127.0.0.1",
        0,
        0,
        0);

    return service_description;
}
bool SocialService::AddFriend(const shared_ptr<Player>& player, const string& friend_name)
{
    uint64_t friend_id = 0;
    try {
        auto conn = kernel()->GetDatabaseManager()->getConnection("galaxy");
        auto statement = std::unique_ptr<sql::PreparedStatement>(
            conn->prepareStatement("SELECT id FROM object where custom_name like ? and type_id = ?;")
            );
        statement->setString(1, friend_name + '%');
        statement->setUInt(2, player->GetType());
        auto result_set = std::unique_ptr<sql::ResultSet>(statement->executeQuery());
        if (result_set->next())
        {
           friend_id = result_set->getUInt64(1);
        }

    } catch(sql::SQLException &e) {
        BOOST_LOG_TRIVIAL(error) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        BOOST_LOG_TRIVIAL(error) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
    /// If we found our friend, lets add them to our friends list (which will get updated by the player)
    if (friend_id > 0)
    {
        player->AddFriend(friend_name);
        // This persists the player object immediately.
        kernel()->GetServiceManager()->GetService<swganh::simulation::SimulationService>
            ("SimulationService")->PersistObject(player->GetObjectId());

        return true;
    }

    return false;
}

void SocialService::onStart()
{
}