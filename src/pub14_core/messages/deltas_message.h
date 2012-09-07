// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include "swganh/byte_buffer.h"
#include "base_deltas_message.h"

namespace swganh {
namespace messages {
    
    class DeltasMessage : public BaseDeltasMessage
    {
    public:
        uint16_t Opcount() const { return 5; }
        uint32_t Opcode() const { return 0x12862153; }
    };
    
}}  // namespace swganh::messages
