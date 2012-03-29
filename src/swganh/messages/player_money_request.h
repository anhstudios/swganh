// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_PLAYER_MONEY_REQUEST_H_
#define SWGANH_MESSAGES_PLAYER_MONEY_REQUEST_H_

#include <cstdint>
#include <string>
#include "anh/byte_buffer.h"
#include "base_swg_message.h"

namespace swganh {
namespace messages {

    struct PlayerMoneyRequest : public BaseSwgMessage<PlayerMoneyRequest>
    {
    	static uint16_t Opcount() { return 1; }
    	static uint32_t Opcode() { return 0x9D105AA1; }
    	
    	void OnSerialize(anh::ByteBuffer& buffer) const {}

    	void OnDeserialize(anh::ByteBuffer buffer) {}
    };

}} // namespace swganh::messages

#endif // SWGANH_MESSAGES_PLAYER_MONEY_REQUEST_H_
