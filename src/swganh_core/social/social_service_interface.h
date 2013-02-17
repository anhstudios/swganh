// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <map>
#include <memory>

#include "swganh/service/service_interface.h"

#include "swganh/app/swganh_kernel.h"
#include "swganh/observer/observer_interface.h"

namespace swganh {
namespace object {
    class Player;
}}  // namespace swganh::object

namespace swganh {
namespace social {
    
    class SocialServiceInterface : public swganh::service::ServiceInterface
    {
    public:
        
        virtual bool AddFriend(const std::shared_ptr<swganh::object::Player>& player, const std::string& friend_name) = 0;
        
		virtual bool AddIgnore(const std::shared_ptr<swganh::object::Player>& player, const std::string& player_name) = 0;
        
    };

}}  // namespace swganh::social
