#include "swganh/social/social_service.h"

#include "anh/logger.h"
#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_manager.h"
#include "anh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh/connection/connection_service.h"
#include "swganh/character/character_provider_interface.h"

#include "swganh/object/player/player.h"
#include "swganh/object/object.h"
#include "swganh/object/object_controller.h"
#include "swganh/object/object_manager.h"

#include "swganh/messages/out_of_band.h"

using namespace anh::database;
using namespace anh::plugin;
using namespace std;
using namespace swganh::connection;
using namespace swganh::object;
using namespace swganh::object::player;
using namespace swganh::social;

using anh::app::KernelInterface;
using anh::service::ServiceDescription;
using swganh::app::SwganhKernel;
using swganh::base::BaseService;
using swganh::character::CharacterProviderInterface;

SocialService::SocialService(SwganhKernel* kernel)
    : BaseService(kernel)
{
    character_provider_ = kernel->GetPluginManager()->CreateObject<CharacterProviderInterface>("CharacterService::CharacterProvider");
}

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
    uint64_t friend_id = character_provider_->GetCharacterIdByName(friend_name);
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