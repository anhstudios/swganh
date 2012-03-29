// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_MESSAGES_BASE_SWG_MESSAGE_H_
#define SWGANH_MESSAGES_BASE_SWG_MESSAGE_H_

#include <cassert>
#include <cstdint>

#include "anh/byte_buffer.h"
#include "anh/utilities.h"

namespace swganh {
namespace messages {
      
    template<typename T>
    struct BaseSwgMessage 
    {
        void Serialize(anh::ByteBuffer& buffer) const
        {
            buffer.write(anh::hostToLittle(T::Opcount()));
            buffer.write(T::Opcode());
            
            auto concrete_message = static_cast<const T* const>(this);
            concrete_message->OnSerialize(buffer);
        }
    
        void Deserialize(anh::ByteBuffer buffer)
        {
            uint32_t Opcount = anh::littleToHost(buffer.read<uint16_t>()); // Opcount
            uint32_t Opcode = buffer.read<uint32_t>();
    
            if (Opcount != T::Opcount() && Opcode != T::Opcode())
            {
                assert(true && "Opcodes don't match, expected: " + Opcode);
                return;
            }
            
            T* concrete_message = static_cast<T*>(this);
            concrete_message->OnDeserialize(std::move(buffer));
        }
    };
        
}}  // namespace swganh::messages

#endif  // SWGANH_MESSAGES_BASE_SWG_MESSAGE_H_
