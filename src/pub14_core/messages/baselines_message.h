// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

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
