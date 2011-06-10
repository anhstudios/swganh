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
#include <list>
#include <gtest/gtest.h>
#include "anh/network/soe/packet_utilities.h"
#include "anh/byte_buffer.h"
#include "anh/utilities.h"

using namespace anh;
using namespace network::soe;
namespace anh {
namespace network {
namespace soe {

class PacketUtilitiesTests : public testing::Test
{
public:
    ByteBuffer expected_buffer, in_buffer, out_buffer;
    std::list<ByteBuffer> buffer_list;
    virtual void SetUp()
    {
        expected_buffer.clear();
        in_buffer.clear();
        out_buffer.clear();
        buffer_list.clear();
    }
    virtual void TearDown() {}
    void buildOutputBuffer()
    {
        expected_buffer.write<uint16_t>(anh::bigToHost<uint16_t>(0x19)); // header
        expected_buffer.write<uint8_t>(4); // size
        expected_buffer.write<int32_t>(5); // value
        expected_buffer.write<uint8_t>(4); // size
        expected_buffer.write<int32_t>(5); // value
        expected_buffer.write<uint8_t>(4); // size
        expected_buffer.write<int32_t>(5); // value
    }
};

TEST_F(PacketUtilitiesTests, packDataChannelFromSingleMessage)
{
    in_buffer.write<int>(5);
    buffer_list.push_back(in_buffer);

    packDataChannelMessages(buffer_list, out_buffer);

    EXPECT_EQ(5, out_buffer.read<int>());
}
TEST_F(PacketUtilitiesTests, packetDataFromMessages)
{
    in_buffer.write<int>(5);

    std::list<ByteBuffer> buffer_list;
    buffer_list.push_back(in_buffer);
    buffer_list.push_back(in_buffer);
    buffer_list.push_back(in_buffer);

    buildOutputBuffer();

    packDataChannelMessages(buffer_list, out_buffer);

    EXPECT_EQ(expected_buffer.size(), out_buffer.size());
    EXPECT_EQ(anh::bigToHost<uint16_t>(0x19), out_buffer.read<uint16_t>());
}

}  // namespace soe
}  // namespace network
}  // namespace anh
