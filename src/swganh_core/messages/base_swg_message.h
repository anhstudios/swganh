// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cassert>
#include <cstdint>

#include "swganh/byte_buffer.h"
#include "swganh/utilities.h"

namespace swganh {
namespace messages {
      
    struct BaseSwgMessage 
    {
		virtual uint16_t Opcount() const = 0;
		virtual uint32_t Opcode() const = 0;
		virtual void OnSerialize(swganh::ByteBuffer& buffer) const = 0;
		virtual void OnDeserialize(swganh::ByteBuffer& buffer) = 0;

        virtual void Serialize(swganh::ByteBuffer& buffer) const
        {
            buffer.write(swganh::hostToLittle(Opcount()));
            buffer.write(Opcode());
            
            OnSerialize(buffer);
        }
    
        virtual void Deserialize(swganh::ByteBuffer buffer)
        {
            uint32_t opcount = swganh::littleToHost(buffer.read<uint16_t>()); // Opcount
            uint32_t opcode = buffer.read<uint32_t>();
    
            if (opcount != Opcount() && opcode != Opcode())
            {
                assert(true && "Opcodes don't match, expected: " + Opcode());
                return;
            }
            
            OnDeserialize(buffer);
        }
    };
        
}}  // namespace swganh::messages
