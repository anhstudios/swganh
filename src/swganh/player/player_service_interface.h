 //This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "anh/service/service_interface.h"

namespace swganh {
namespace object {
namespace player {
	class Player;
}}}


namespace swganh {
namespace player {

class PlayerServiceInterface : anh::service::ServiceInterface
{
public:
	virtual void CleanupPlayerState(std::shared_ptr<swganh::object::player::Player> player) = 0;

	virtual void OnPlayerEnter(std::shared_ptr<swganh::object::player::Player> player) = 0;

	virtual void OnPlayerExit(std::shared_ptr<swganh::object::player::Player> player) = 0;
};



}}// swganh::player
