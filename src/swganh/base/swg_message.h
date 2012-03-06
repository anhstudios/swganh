/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SWGANH_BASE_SWG_MESSAGE_H_
#define SWGANH_BASE_SWG_MESSAGE_H_

#include <cassert>
#include <cstdint>

#include "anh/byte_buffer.h"
#include "anh/utilities.h"

namespace swganh {
namespace base {
      
template<typename T>
struct SwgMessage {
    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write(anh::hostToLittle(T::opcount()));
        buffer.write(T::opcode());
        
        onSerialize(buffer);
    }
    
    void deserialize(anh::ByteBuffer buffer) {
        anh::littleToHost(buffer.read<uint16_t>());  // opcount
        uint32_t opcode = buffer.read<uint32_t>();

        if (opcode != T::opcode()) {
            assert(true && "opcodes don't match, expected:" + opcode );
            return;
        }
        
        onDeserialize(std::move(buffer));
    }    
    
    virtual void onSerialize(anh::ByteBuffer& buffer) const = 0;
    virtual void onDeserialize(anh::ByteBuffer buffer) = 0;
};
        
}}  // namespace swganh::base

#endif  // SWGANH_BASE_SWG_MESSAGE_H_
