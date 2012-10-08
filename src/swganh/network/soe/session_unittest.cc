// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>

#include "swganh/byte_buffer.h"
#include "swganh/network/soe/session.h"

#include "swganh/network/soe/mock_server.h"

using namespace swganh::network;
using namespace boost::asio::ip;
using namespace std;

namespace swganh {
namespace network {
namespace soe {

class SessionTests {
protected:
    // builds a simple swg message
    ByteBuffer buildSimpleMessage() const;

    // builds a simple data channel packet from buildSimpleMessage with the given sequence
    ByteBuffer buildSimpleDataChannelPacket(uint16_t sequence) const;

    ByteBuffer buildSimpleFragmentedMessage() const;

    ByteBuffer buildSimpleFragmentedPacket(uint16_t sequence) const;

    udp::endpoint buildTestEndpoint() const;

    shared_ptr<MockServer> buildMockServer() const;
};

BOOST_FIXTURE_TEST_SUITE(SessionTest, SessionTests)

/// This test verifies that new sessions have a send sequence of 0
BOOST_AUTO_TEST_CASE(NewSessionHasZeroSendSequence) {
    auto service = buildMockServer();
    boost::asio::io_service io_service;
    shared_ptr<Session> session = make_shared<Session>(service.get(), io_service, buildTestEndpoint());

    BOOST_CHECK_EQUAL(0, session->server_sequence());
}

/// This test verifies that data packets sent out on the data channel are sequenced.
BOOST_AUTO_TEST_CASE(SendingDataChannelMessageIncreasesServerSequence) {
    auto service = buildMockServer();
    boost::asio::io_service io_service;
    shared_ptr<Session> session = make_shared<Session>(service.get(), io_service, buildTestEndpoint());

    // Send 3 data channel messages and ensure the sequence is increased appropriately.
    for (int i = 1; i <= 3; ++i ) {
        session->SendTo(buildSimpleMessage());
		session->Update();
        BOOST_CHECK_EQUAL(i, session->server_sequence());
    }
}

/// This test verifies that data channel messages are stored in case they need to be re-sent.
BOOST_AUTO_TEST_CASE(DataChannelMessagesAreStoredForResending) {
    auto service = buildMockServer();
    boost::asio::io_service io_service;
    shared_ptr<Session> session = make_shared<Session>(service.get(), io_service, buildTestEndpoint());

    // Send 3 data channel messages.
    for (int i = 1; i <= 3; ++i ) {
        session->SendTo(buildSimpleMessage());
		session->Update();
    }

    auto sent_messages = session->GetUnacknowledgedMessages();

    // Expect the vector of sent messages to contain 3 elements
    BOOST_CHECK_EQUAL(3, sent_messages.size());
}
BOOST_AUTO_TEST_SUITE_END()

// SessionTest member implementations

ByteBuffer SessionTests::buildSimpleMessage() const {
    ByteBuffer buffer;

    buffer.write<uint16_t>(1);
    buffer.write<uint32_t>(0xDEADBABE);

    return buffer;
}

ByteBuffer SessionTests::buildSimpleDataChannelPacket(uint16_t sequence) const {
    ByteBuffer buffer;

    buffer.write<uint16_t>(hostToBig<uint16_t>(0x09));
    buffer.write<uint16_t>(hostToBig<uint16_t>(sequence));
    buffer.append(buildSimpleMessage());

    return buffer;
}

ByteBuffer SessionTests::buildSimpleFragmentedMessage() const {
    ByteBuffer buffer;

    for (int i = 0; i < 6; ++i) {
        buffer.write<uint32_t>(5);
    }

    return buffer;
}

ByteBuffer SessionTests::buildSimpleFragmentedPacket(uint16_t sequence) const {
    ByteBuffer buffer;

    buffer.write<uint16_t>(hostToBig<uint16_t>(0x0D));
    buffer.write<uint16_t>(hostToBig<uint16_t>(sequence));
    buffer.write<uint32_t>(hostToBig<uint32_t>(24)); // size of the total fragmented message

    for (int i = 0; i < 4; ++i) {
        buffer.write<uint32_t>(5);
    }

    return buffer;
}

udp::endpoint SessionTests::buildTestEndpoint() const {
    udp::endpoint endpoint(address_v4::from_string("127.0.0.1"), 1000);
    return endpoint;
}


shared_ptr<MockServer> SessionTests::buildMockServer() const {
    auto server = make_shared<MockServer>();

    MOCK_EXPECT(server->max_receive_size)
        .at_least(1)
        .returns(496);
        
    return server;
}

}}}  // namespace swganh::network::soe
