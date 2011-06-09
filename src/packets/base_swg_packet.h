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

#ifndef PACKETS_BASE_SWG_MESSAGE_H_
#define PACKETS_BASE_SWG_MESSAGE_H_

namespace packets {
  
template<typename T>
struct BaseSwgPacket {
    void serialize(anh::ByteBuffer& buffer) const {
        buffer.write(T::opcount);
        buffer.write(T::opcode);
        
        onSerialize(buffer);
    }
    
    void deserialize(anh::ByteBuffer buffer) {
        uint16_t opcount = buffer.read<uint16_t>();
        uint32_t opcode = buffer.read<uint32_t>();

        if (opcount != T::opcount || opcode != T::opcode) {
            // @todo throw here perhaps? for now just return;
            return;
        }
        
        onDeserialize(std::move(buffer));
    }    
    
    virtual void onSerialize(anh::ByteBuffer& buffer) const = 0;
    virtual void onDeserialize(anh::ByteBuffer buffer) = 0;
};
        
}  // namespace packets

#endif  // PACKETS_BASE_SWG_MESSAGE_H_
