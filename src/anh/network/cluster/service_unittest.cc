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
#include <anh/network/cluster/cluster_service.h>
#include <anh/server_directory/mock_server_directory.h>
#include <anh/event_dispatcher/mock_event_dispatcher.h>

using namespace anh;
using namespace server_directory;
using namespace event_dispatcher;
using namespace network::cluster;

namespace {
class ClusterServiceTest : public testing::Test
{
protected:
    std::shared_ptr<MockEventDispatcher> dispatcher;
    std::shared_ptr<MockServerDirectory> directory;
    boost::asio::io_service io_service;
    std::shared_ptr<ClusterService>    service;

    virtual void SetUp()
    {
        dispatcher = std::make_shared<MockEventDispatcher>();
        directory = std::make_shared<MockServerDirectory>();
        service = std::make_shared<ClusterService>(io_service, directory, dispatcher, 44993);
    }
    virtual void TearDown()
    {

    }
};

TEST_F(ClusterServiceTest, sendMessage ) 
{

    EXPECT_TRUE(true);
}

}