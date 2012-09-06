// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cassert>
#include <cstdint>

#include "anh/byte_buffer.h"
#include "anh/utilities.h"

namespace swganh {
namespace messages {
      
    struct BaseSwgMessage 
    {
		virtual uint16_t Opcount() const = 0;
		virtual uint32_t Opcode() const = 0;
		virtual void OnSerialize(anh::ByteBuffer& buffer) const = 0;
		virtual void OnDeserialize(anh::ByteBuffer& buffer) = 0;

        virtual void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(anh::hostToLittle(Opcount()));
            buffer.write(Opcode());
            
            OnSerialize(buffer);
        }
    
        virtual void Deserialize(anh::ByteBuffer buffer)
        {
            uint32_t opcount = anh::littleToHost(buffer.read<uint16_t>()); // Opcount
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
