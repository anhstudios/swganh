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
#include <gmock/gmock.h>
#include <boost/thread.hpp>
#include "anh/network/cluster/cluster_service.h"
#include "anh/server_directory/mock_server_directory.h"
#include "anh/event_dispatcher/mock_event_dispatcher.h"
#include "anh/event_dispatcher/basic_event.h"
#include "packets/NetworkEventMessage.h"

using namespace anh;
using namespace server_directory;
using namespace event_dispatcher;
using namespace network::cluster;
using namespace testing;
using namespace packets;

namespace {
class ClusterServiceTest : public testing::Test
{
protected:
    std::shared_ptr<NiceMock<MockEventDispatcher>>  dispatcher;
    std::shared_ptr<MockServerDirectory>            directory;
    boost::asio::io_service io_service, host_io_service;
    std::shared_ptr<ClusterService>                 service, host_service;
    std::shared_ptr<Process>                        test_process;
    NetworkEventMessage nem;

    virtual void SetUp()
    {
        dispatcher = std::make_shared<NiceMock<MockEventDispatcher>>();
        directory = std::make_shared<MockServerDirectory>();
        service = std::make_shared<ClusterService>(io_service, directory, dispatcher, 44993);
        host_service = std::make_shared<ClusterService>(host_io_service, directory, dispatcher, 44994);
        test_process = std::make_shared<Process>(0, 1, "ANH.Test", "Test", "0.1","127.0.0.1", 44994, 0, 0);
    }
    virtual void TearDown()
    {

    }
    void setupNetworkEventMessage()
    {
        nem.string_message = "Hello World!";
    }
};
// verifies that if we open a connection, it shows up in our map
TEST_F(ClusterServiceTest, openConnection) 
{
    service->Connect(test_process);
    EXPECT_TRUE(service->isConnected(test_process));
}
// verifies that in debug
TEST_F(ClusterServiceTest, cantSendMessageWithNoConnections)
{
    EXPECT_DEBUG_DEATH(
        service->sendMessage("ANH.Test", make_shared_event(NetworkEventMessage::hash_type(), nem))
        , "No Connection Established, could not send message to:");
}
TEST_F(ClusterServiceTest, canSendMessageWithConnection)
{
    
}

}