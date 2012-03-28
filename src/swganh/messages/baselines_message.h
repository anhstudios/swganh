// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_BASELINES_MESSAGE_H_
#define SWGANH_MESSAGES_BASELINES_MESSAGE_H_

#include <cstdint>

#include "anh/byte_buffer.h"
#include "base_baselines_message.h"

namespace swganh {
namespace messages {
    
    struct BaselinesMessage : public BaseBaselinesMessage<BaselinesMessage>
    {
        static uint16_t Opcount() { return 5; }
        static uint32_t Opcode() { return 0x68A75F0C; }
    };
    
}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_BASELINES_MESSAGE_H_
