// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "social_service.h"

#include "swganh/logger.h"
#include "swganh/plugin/plugin_manager.h"
#include "swganh/service/service_manager.h"
#include "swganh/database/database_manager.h"

#include "swganh/app/swganh_kernel.h"

#include "swganh_core/connection/connection_service_interface.h"
#include "swganh_core/character/character_provider_interface.h"

#include "swganh_core/object/player/player.h"
#include "swganh_core/object/object.h"
#include "swganh/observer/observer_interface.h"
#include "swganh_core/object/object_manager.h"

#include "swganh_core/messages/out_of_band.h"

using namespace swganh::database;
using namespace swganh::plugin;
using namespace std;
using namespace swganh::connection;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::social;

using swganh::app::KernelInterface;
using swganh::service::ServiceDescription;
using swganh::app::SwganhKernel;
using swganh::character::CharacterProviderInterface;

SocialService::SocialService(SwganhKernel* kernel)
    : kernel_(kernel)
{
    character_provider_ = kernel->GetPluginManager()->CreateObject<CharacterProviderInterface>("Character::CharacterProvider");
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
        player->AddFriend(friend_name, friend_id);
        return true;
    }

    return false;
}

bool SocialService::AddIgnore(const shared_ptr<Player>& player, const string& player_name)
{
    uint64_t player_id = character_provider_->GetCharacterIdByName(player_name);
    /// If we found the player name, lets add them to our ignore list (which will get updated by the player)
    if (player_id > 0)
    {
        player->IgnorePlayer(player_name, player_id);
        return true;
    }

    return false;
}
