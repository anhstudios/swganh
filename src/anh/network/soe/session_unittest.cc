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

#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <gtest/gtest.h>

#include "anh/byte_buffer.h"
#include "anh/network/soe/session.h"

#include "anh/network/mock_socket.h"

using namespace anh::network;
using namespace boost::asio::ip;
using namespace std;

using testing::_;
using testing::NiceMock;

namespace anh {
namespace network {
namespace soe {

class SessionTests : public ::testing::Test {
protected:
    // builds a simple swg message
    ByteBuffer buildSimpleMessage() const;

    // builds a simple data channel packet from buildSimpleMessage with the given sequence
    ByteBuffer buildSimpleDataChannelPacket(uint16_t sequence) const;

    udp::endpoint buildTestEndpoint() const;
};

/// This test verifies that new sessions have a send sequence of 0
TEST_F(SessionTests, NewSessionHasZeroSendSequence) {
    NiceMock<MockSocket<udp>> socket;
    Session session(buildTestEndpoint(), &socket);

    EXPECT_EQ(0, session.server_sequence());
}

/// This test verifies that data packets sent out on the data channel are sequenced.
TEST_F(SessionTests, SendingDataChannelMessageIncreasesServerSequence) {
    NiceMock<MockSocket<udp>> socket;
    Session session(buildTestEndpoint(), &socket);

    // Send 3 data channel messages and ensure the sequence is increased appropriately.
    for (int i = 1; i <= 3; ++i ) {
        session.SendMessage(buildSimpleMessage());
        EXPECT_EQ(i, session.server_sequence());
    }
}

/// This test verifies that data channel messages are sent out via the soe service
TEST_F(SessionTests, DataChannelMessagesAreSentViaSoeService) {
    MockSocket<udp> socket;

    EXPECT_CALL(socket, Send(_, _))
        .Times(1);
    
    Session session(buildTestEndpoint(), &socket);

    // Send a data channel message.
    session.SendMessage(buildSimpleMessage());
}

/// This test verifies that data channel messages are stored in case they need to be re-sent.
TEST_F(SessionTests, DataChannelMessagesAreStoredForResending) {
    NiceMock<MockSocket<udp>> socket;
    Session session(buildTestEndpoint(), &socket);

    // Send 3 data channel messages.
    for (int i = 1; i <= 3; ++i ) {
        session.SendMessage(buildSimpleMessage());
    }

    vector<shared_ptr<ByteBuffer>> sent_messages = session.GetUnacknowledgedMessages();

    // Expect the vector of sent messages to contain 3 elements
    EXPECT_EQ(3, sent_messages.size());
}

/// This test verifies that data channel messages are wrapped in the proper header.
TEST_F(SessionTests, DataChannelMessagesAreWrappedInDataHeaderWhenSent) {
    MockSocket<udp> socket;

    EXPECT_CALL(socket, Send(_, buildSimpleDataChannelPacket(1)))
        .Times(1);
    
    Session session(buildTestEndpoint(), &socket);

    // Send a data channel message.
    session.SendMessage(buildSimpleMessage());
}


TEST_F(SessionTests, LargeDataChannelMessagesAreWrappedInFragmentedHeaderWhenSent) {}


// SessionTest member implementations

ByteBuffer SessionTests::buildSimpleMessage() const {
    ByteBuffer buffer;

    buffer.write<uint16_t>(1);
    buffer.write<uint32_t>(0xDEADBABE);

    return buffer;
}

ByteBuffer SessionTests::buildSimpleDataChannelPacket(uint16_t sequence) const {
    ByteBuffer buffer;

    buffer.write<uint16_t>(anh::hostToBig<uint16_t>(0x09));
    buffer.write<uint16_t>(anh::hostToBig<uint16_t>(sequence));
    buffer.append(buildSimpleMessage());

    return buffer;
}

udp::endpoint SessionTests::buildTestEndpoint() const {
    udp::endpoint endpoint(address_v4::from_string("127.0.0.1"), 1000);
    return endpoint;
}

}}}  // namespace anh::network::soe
