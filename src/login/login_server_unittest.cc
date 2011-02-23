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

#include <algorithm>
#include <memory>
#include "anh/event_dispatcher/mock_event_dispatcher.h"
#include "anh/server_directory/mock_server_directory.h"

#include "login/login_server.h"

using namespace anh::event_dispatcher;
using namespace anh::server_directory;
using namespace std;
using namespace testing;

using login::LoginServer;

class LoginServerTest : public Test {
protected:
    shared_ptr<MockEventDispatcher> buildMockEventDispatcher();
};

/// This test verifies that the login server listens for LoginClientId events.
TEST_F(LoginServerTest, ListensForLoginClientIdEvents) {
    auto mock_dispatcher = buildMockEventDispatcher();
    EXPECT_CALL(*mock_dispatcher, subscribe(EventType("LoginClientId"), _))
        .WillOnce(Return(true));
    
    LoginServer login_server;
    login_server.event_dispatcher(mock_dispatcher);
    login_server.startup();
}

/// This test verifies that the login server lists for 
/// DeleteCharacterMessage events.
TEST_F(LoginServerTest, ListensForDeleteCharacterMessageEvents) {
    auto mock_dispatcher = buildMockEventDispatcher();
    EXPECT_CALL(*mock_dispatcher, 
                subscribe(EventType("DeleteCharacterMessage"), _))
        .WillOnce(Return(true));

    LoginServer login_server;
    login_server.event_dispatcher(mock_dispatcher);
    login_server.startup();
}

/// This test verifies that the login server maintains a list of the cluster
/// versions.
TEST_F(LoginServerTest, ChecksClusterMapForAvailableVersions) {
    Cluster test_cluster1(
        1, 
        2, 
        "test_cluster1", 
        "20050408-18:00", 
        Cluster::OFFLINE, 
        "1970-01-01 00:00:01", 
        "1970-01-01 00:00:01");

    Cluster test_cluster2(
        1, 
        2, 
        "test_cluster2", 
        "20050408-19:00", 
        Cluster::OFFLINE, 
        "1970-01-01 00:00:01", 
        "1970-01-01 00:00:01");

    Cluster test_cluster3(
        1, 
        2, 
        "test_cluster3", 
        "20050408-20:00", 
        Cluster::OFFLINE, 
        "1970-01-01 00:00:01", 
        "1970-01-01 00:00:01");

    std::list<Cluster> cluster_list;
    cluster_list.push_back(test_cluster1);
    cluster_list.push_back(test_cluster2);
    cluster_list.push_back(test_cluster3);
    
    LoginServer login_server;
    list<string> versions = login_server.getAvailableVersions(cluster_list);

    if (cluster_list.size() != versions.size()) {
        FAIL();
        return;
    }

    EXPECT_TRUE(std::equal(
        cluster_list.begin(),
        cluster_list.end(),
        versions.begin(),          
        [] (const Cluster& cluster, const string& version) 
    {
        return version.compare(cluster.version()) == 0;
    }));
}

/// This test confirms that the login server grabs a list of available versions
/// whenever it updates the cluster list.
TEST_F(LoginServerTest, BuildsVersionListWhenUpdatingClusterList) {
    shared_ptr<MockServerDirectory> mock_server_directory(
        new MockServerDirectory());

    Cluster test_cluster1(
        1, 
        2, 
        "test_cluster1", 
        "20050408-18:00", 
        Cluster::OFFLINE, 
        "1970-01-01 00:00:01", 
        "1970-01-01 00:00:01");

    Cluster test_cluster2(
        1, 
        2, 
        "test_cluster2", 
        "20050408-19:00", 
        Cluster::OFFLINE, 
        "1970-01-01 00:00:01", 
        "1970-01-01 00:00:01");

    Cluster test_cluster3(
        1, 
        2, 
        "test_cluster3", 
        "20050408-20:00", 
        Cluster::OFFLINE, 
        "1970-01-01 00:00:01", 
        "1970-01-01 00:00:01");

    std::list<Cluster> cluster_list;
    cluster_list.push_back(test_cluster1);
    cluster_list.push_back(test_cluster2);
    cluster_list.push_back(test_cluster3);

    EXPECT_CALL(*mock_server_directory, getClusterSnapshot())
        .WillOnce(Return(cluster_list));

    LoginServer login_server;
    login_server.server_directory(mock_server_directory);
    login_server.updateClusterList();

    list<string> versions = login_server.getAvailableVersions();

    if (cluster_list.size() != versions.size()) {
        FAIL();
        return;
    }

    EXPECT_TRUE(std::equal(
        cluster_list.begin(),
        cluster_list.end(),
        versions.begin(),          
        [] (const Cluster& cluster, const string& version) 
    {
        return version.compare(cluster.version()) == 0;
    }));
}

shared_ptr<MockEventDispatcher> LoginServerTest::buildMockEventDispatcher() {
   shared_ptr<MockEventDispatcher> mock_dispatcher(
       new NiceMock<MockEventDispatcher>());
    
    EXPECT_CALL(*mock_dispatcher, subscribe(_, _))
        .WillRepeatedly(Return(true));

    return mock_dispatcher;
}
