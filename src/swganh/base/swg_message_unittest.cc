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

#include <gtest/gtest.h>

#include "anh/utilities.h"

#include "swganh/base/swg_message.h"

using swganh::base::SwgMessage;

namespace {
    
struct DataMessage : public SwgMessage<DataMessage> {
    static uint16_t opcount() { return 1; }
    static uint32_t opcode() { return 0x5BE5CD7; }
    
    uint32_t test_value;

    void onDeserialize(anh::ByteBuffer buffer) {
        test_value = buffer.read<uint32_t>();    
    }
    
    void onSerialize(anh::ByteBuffer& buffer) const {
        buffer.write(test_value);
    }
};

TEST(SwgMessageTest, SwgMessagesAreSerializable) {
    
    DataMessage message;
    
    message.test_value = 500;
         
    anh::ByteBuffer buffer;
    message.serialize(buffer);
    
    EXPECT_EQ(anh::littleToHost<uint16_t>(1), buffer.read<uint16_t>());
    EXPECT_EQ(0x5BE5CD7, buffer.read<uint32_t>());
    EXPECT_EQ(500, buffer.read<uint32_t>());
}

}  // namespace
