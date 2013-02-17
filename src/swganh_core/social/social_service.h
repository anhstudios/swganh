// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "swganh_core/social/social_service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/observer/observer_interface.h"

namespace swganh {
namespace character{
	class CharacterProviderInterface;
}
namespace social {
    
    class SocialService : public swganh::social::SocialServiceInterface
    {
    public:
        explicit SocialService(swganh::app::SwganhKernel* kernel);
    
        ~SocialService();

        swganh::service::ServiceDescription GetServiceDescription();

        virtual bool AddFriend(const std::shared_ptr<swganh::object::Player>& player, const std::string& friend_name);
        virtual bool AddIgnore(const std::shared_ptr<swganh::object::Player>& player, const std::string& player_name);
        
    private:
        std::shared_ptr<swganh::character::CharacterProviderInterface> character_provider_;
        swganh::app::SwganhKernel* kernel_;
    };

}}  // namespace swganh::social
