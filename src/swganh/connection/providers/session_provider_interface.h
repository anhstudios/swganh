// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

namespace swganh {
namespace connection {
namespace providers {

class SessionProviderInterface {
public:
    virtual ~SessionProviderInterface() {}

    virtual uint64_t GetPlayerId(uint32_t account_id) = 0;
    virtual uint32_t GetAccountId(uint64_t player_id) = 0;
    virtual bool CreateGameSession(uint64_t player_id, uint32_t session_id) = 0;
	virtual void EndGameSession(uint64_t player_id) = 0;
};

}}}  // namespace swganh::connection::providers
