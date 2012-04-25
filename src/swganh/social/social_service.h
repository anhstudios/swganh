// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_H_

#include <cstdint>
#include <map>
#include <memory>

#include "anh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/object/object_controller.h"

namespace swganh {
namespace object {
    class Object;
namespace player {
    class Player;
}
}}  // namespace swganh::object

namespace swganh {
namespace character {
class CharacterProviderInterface;
}} // namespace swganh::character

namespace swganh {
namespace social {
    
    class SocialService : public anh::service::ServiceInterface
    {
    public:
        explicit SocialService(swganh::app::SwganhKernel* kernel);
    
        ~SocialService();

        anh::service::ServiceDescription GetServiceDescription();

        bool AddFriend(const std::shared_ptr<swganh::object::player::Player>& player, const std::string& friend_name);
        bool AddIgnore(const std::shared_ptr<swganh::object::player::Player>& player, const std::string& player_name);
        
    private:
        std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;
        swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::social

#endif  // SWGANH_SOCIAL_SOCIAL_SERVICE_H_
