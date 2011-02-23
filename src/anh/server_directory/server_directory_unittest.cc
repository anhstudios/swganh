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

#include <boost/date_time/posix_time/posix_time.hpp>

#include "anh/server_directory/cluster.h"
#include "anh/server_directory/datastore.h"
#include "anh/server_directory/process.h"
#include "anh/server_directory/server_directory.h"

using namespace anh::server_directory;
using namespace std;
using namespace boost::posix_time;
using namespace testing;


// Test harness for the ServerDirectory unit tests, provides a database_connection_
// to use for testing.
class ServerDirectoryTest : public testing::Test {
protected:
    virtual void SetUp() {
        test_cluster_ = make_shared<Cluster>(getTestCluster());
        test_process_ = make_shared<Process>(getTestProcess());
    }

    Cluster getTestCluster() {
        Cluster cluster(1, 2, "test_cluster", "20050408-18:00", Cluster::OFFLINE, "1970-01-01 00:00:01", "1970-01-01 00:00:01");
        return cluster;
    }
    
    Process getTestProcess() {
        Process process(1, 1, "process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0);
        return process;
    }

    std::shared_ptr<Cluster> test_cluster_;
    std::shared_ptr<Process> test_process_;
};

class MockDatastore : public DatastoreInterface {
public:
    MOCK_CONST_METHOD1(findClusterByName, shared_ptr<Cluster>(const std::string& name));
    MOCK_CONST_METHOD2(createCluster, shared_ptr<Cluster>(const std::string& name, const std::string& version));
    MOCK_CONST_METHOD8(createProcess, shared_ptr<Process>(std::shared_ptr<Cluster> cluster, const std::string& name, const std::string& type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping_port));
    MOCK_CONST_METHOD1(getClusterTimestamp, std::string(std::shared_ptr<Cluster> cluster));
    MOCK_CONST_METHOD1(saveProcess, void(std::shared_ptr<Process> process));
    MOCK_CONST_METHOD1(findClusterById, shared_ptr<Cluster>(uint32_t id));
    MOCK_CONST_METHOD1(deleteProcessById, bool(uint32_t id));
    MOCK_CONST_METHOD0(getClusterList, ClusterList());
    MOCK_CONST_METHOD1(getProcessList, ProcessList(uint32_t cluster_id));
    MOCK_CONST_METHOD1(prepareTimestampForStorage, std::string(const std::string& timestamp));
};

/// Creating and using an instance of ServerDirectory requires a valid cluster
/// so the process of constructing should join with a cluster.
TEST_F(ServerDirectoryTest, CreatingServerDirectoryJoinsToCluster) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("test_cluster"))
        .WillOnce(Return(test_cluster_));
        
    try {
        ServerDirectory server_directory(datastore, "test_cluster", "20050408-18:00");
    
        auto cluster = server_directory.cluster();

        EXPECT_EQ("test_cluster", cluster->name());
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful join";
    }
}

/// When creating an instance of ServerDirectory with an invalid cluster name
/// an InvalidCluster exception is thrown.
TEST_F(ServerDirectoryTest, JoiningInvalidClusterThrowsException) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("bad_cluster"))
        .WillOnce(Return(nullptr));

    try {
        ServerDirectory server_directory(datastore, "bad_cluster", "20050408-18:00");
        FAIL() << "An exception should be thrown when given an invalid cluster name";
    } catch(const InvalidClusterError& /*e*/) {
        SUCCEED();
    }
}

/// Passing an optional parameter to the constructor while create a cluster
/// and set it as the active cluster.
TEST_F(ServerDirectoryTest, CanCreateClusterWhenJoining) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("test_cluster"))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*datastore, createCluster("test_cluster", "20050408-18:00"))
        .WillOnce(Return(test_cluster_));
    
    try {
        ServerDirectory server_directory(datastore, "test_cluster", "20050408-18:00", true);
        
        auto cluster = server_directory.cluster();

        EXPECT_EQ("test_cluster", cluster->name());
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

/// Registering a process makes it the active process.
TEST_F(ServerDirectoryTest, RegisteringProcessMakesItActive) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("test_cluster"))
        .WillOnce(Return(test_cluster_));

    EXPECT_CALL(*datastore, createProcess(_, "process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_process_));

    try {
        ServerDirectory server_directory(datastore, "test_cluster", "20050408-18:00");

        EXPECT_TRUE(server_directory.registerProcess("process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0));

        auto process = server_directory.process();
        
        EXPECT_EQ("process_name", process->name());
        EXPECT_EQ("test_process", process->type());
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

TEST_F(ServerDirectoryTest, CanMakeActiveProcessThePrimaryClusterProcess) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("test_cluster"))
        .WillOnce(Return(test_cluster_));

    EXPECT_CALL(*datastore, createProcess(_, "process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_process_));

    try {
        ServerDirectory server_directory(datastore, "test_cluster", "20050408-18:00");

        EXPECT_TRUE(server_directory.registerProcess("process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0));
        
        auto cluster = server_directory.cluster();
        auto process = server_directory.process();

        EXPECT_NE(cluster->primary_id(), process->id());

        EXPECT_TRUE(server_directory.makePrimaryProcess(process));
        
        cluster = server_directory.cluster();
        process = server_directory.process();

        EXPECT_EQ(cluster->primary_id(), process->id());

    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

TEST_F(ServerDirectoryTest, PulsingUpdatesActiveProcessTimestamp) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("test_cluster"))
        .WillOnce(Return(test_cluster_));

    EXPECT_CALL(*datastore, createProcess(_, "process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_process_));

    EXPECT_CALL(*datastore, getClusterTimestamp(_))
        .WillOnce(Return(std::string("1970-01-01 00:01:01")));

    EXPECT_CALL(*datastore, saveProcess(test_process_))
        .Times(1);
    
    EXPECT_CALL(*datastore, findClusterById(test_cluster_->id()))
        .WillOnce(Return(test_cluster_));

    try {
        ServerDirectory server_directory(datastore, "test_cluster", "20050408-18:00");

        EXPECT_TRUE(server_directory.registerProcess("process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0));
        
        auto process = server_directory.process();
        std::string registration_time = process->last_pulse();
        
        server_directory.pulse();
                
        process = server_directory.process();
        std::string pulse_time = process->last_pulse();
        
        EXPECT_NE(registration_time, pulse_time);
        EXPECT_LT(ptime(time_from_string(registration_time)),
                  ptime(time_from_string(pulse_time)));
    } catch(const std::exception& e) {
        FAIL() << "No exceptions should be thrown during a successful create/join" << e.what();
    }
}

/// When deleting a process it should be nullified
TEST_F(ServerDirectoryTest, RemovingProcessNullifiesIt) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("test_cluster"))
        .WillOnce(Return(test_cluster_));

    EXPECT_CALL(*datastore, createProcess(_, "process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_process_));
    
    EXPECT_CALL(*datastore, deleteProcessById(1))
        .WillOnce(Return(true));

    try {
        ServerDirectory server_directory(datastore, "test_cluster", "20050408-18:00");

        EXPECT_TRUE(server_directory.registerProcess("process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0));

        auto process = server_directory.process();
        
        EXPECT_EQ("process_name", process->name());
        EXPECT_EQ("test_process", process->type());

        server_directory.removeProcess(std::move(process));

        EXPECT_EQ(nullptr, process);

    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

/// When deleting a process it should be nullified, if it's the active process
/// then the instance held by ServerDirectory needs to be updated as well.
TEST_F(ServerDirectoryTest, RemovingActiveProcessNullifiesIt) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findClusterByName("test_cluster"))
        .WillOnce(Return(test_cluster_));

    EXPECT_CALL(*datastore, createProcess(_, "process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_process_));

    EXPECT_CALL(*datastore, deleteProcessById(1))
        .WillOnce(Return(true));

    try {
        ServerDirectory server_directory(datastore, "test_cluster", "20050408-18:00");

        EXPECT_TRUE(server_directory.registerProcess("process_name", "test_process", "1.0.0", "127.0.0.1", 0, 40000, 0));

        auto process = server_directory.process();
        
        EXPECT_EQ("process_name", process->name());
        EXPECT_EQ("test_process", process->type());

        server_directory.removeProcess(process);

        EXPECT_EQ(nullptr, server_directory.process());

    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}
