
#ifndef SWGANH_SOCIAL_SOCIAL_SERVICE_H_
#define SWGANH_SOCIAL_SOCIAL_SERVICE_H_

#include <cstdint>
#include <map>
#include <memory>

#include "swganh/base/base_service.h"
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
    
    class SocialService : public swganh::base::BaseService
    {
    public:
        explicit SocialService(swganh::app::SwganhKernel* kernel);
    
        ~SocialService();

        anh::service::ServiceDescription GetServiceDescription();

        bool AddFriend(const std::shared_ptr<swganh::object::player::Player>& player, const std::string& friend_name);
    private:
        void onStart();
        std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;

    };

}}  // namespace swganh::social

#endif  // SWGANH_SOCIAL_SOCIAL_SERVICE_H_
