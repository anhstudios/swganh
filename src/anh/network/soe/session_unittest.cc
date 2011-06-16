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

using namespace std;

namespace anh {
namespace network {
namespace soe {

class SessionTests : public ::testing::Test {
protected:
    ByteBuffer buildSimpleMessage() const;
};

/// This test verifies that new sessions have a send sequence of 0
TEST_F(SessionTests, NewSessionHasZeroSendSequence) {
    Session session(boost::asio::ip::udp::endpoint(), nullptr);
    EXPECT_EQ(0, session.server_sequence());
}

/// This test verifies that data packets sent out on the data channel are sequenced.
TEST_F(SessionTests, SendingDataChannelMessageIncreasesServerSequence) {
    Session session(boost::asio::ip::udp::endpoint(), nullptr);

    // Send 3 data channel messages and ensure the sequence is increased appropriately.
    for (int i = 1; i <= 3; ++i ) {
        session.sendDataChannelMessage(buildSimpleMessage());
        EXPECT_EQ(i, session.server_sequence());
    }
}

/// This test verifies that data channel messages are stored in case they need to be re-sent.
TEST_F(SessionTests, DataChannelMessagesAreStoredForResending) {
    Session session(boost::asio::ip::udp::endpoint(), nullptr);

    // Send 3 data channel messages.
    for (int i = 1; i <= 3; ++i ) {
        session.sendDataChannelMessage(buildSimpleMessage());
    }

    vector<shared_ptr<ByteBuffer>> sent_messages = session.getUnacknowledgedOutgoingMessages();

    // Expect the vector of sent messages to contain 3 elements
    EXPECT_EQ(3, sent_messages.size());
}


// SessionTest member implementations

ByteBuffer SessionTests::buildSimpleMessage() const {
    ByteBuffer buffer;

    buffer.write<uint16_t>(1);
    buffer.write<uint32_t>(0xDEADBABE);

    return buffer;
}

}}}  // namespace anh::network::soe
