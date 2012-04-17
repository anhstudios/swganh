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

#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>

#include "anh/database/mock_cppconn.h"

#include "anh/database/database_manager.h"

using namespace anh::database; 
using namespace testing;


/// Test fixture for testing the DatabaseManager, provides setup/teardown
class DatabaseManagerTest {
protected:
    void SetUp();
    // Generates expectations for a driver that will return N connections.
    void generateExpectations(NiceMock<MockDriver>& mock_driver, int connections);
    void generateDefaultConnectionExpectations(NiceMock<MockDriver>& mock_driver, MockConnection* mock_connection);

    sql::SQLString host_;
    sql::SQLString username_;
    sql::SQLString password_;
};

BOOST_FIXTURE_TEST_SUITE(DBManager, DatabaseManagerTest)

/// This test shows that a newly created DatabaseManager has no storage types
/// registered by default.
BOOST_AUTO_TEST_CASE(NoStorageTypesRegisteredByDefault) {
    MockDriver mock_driver;
    DatabaseManager manager(&mock_driver);
    
    EXPECT_FALSE(manager.hasStorageType("my_storage_type"));
}

/// This test shows that a newly created DatabaseManager has no connections
/// registered by default.
BOOST_AUTO_TEST_CASE(NoConnectionsByDefault) {
    MockDriver mock_driver;
    DatabaseManager manager(&mock_driver);
    
    EXPECT_FALSE(manager.hasConnection("my_storage_type"));
}

/// This test shows how to register connection data for a storage type.
BOOST_AUTO_TEST_CASE(CanRegisterConnectionDataForStorageType) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    NiceMock<MockDriver> mock_driver;
    generateExpectations(mock_driver, 1);

    // Create the database manager with the mock driver.
    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a registration with valid information";
    }

    EXPECT_TRUE(manager.hasStorageType("my_storage_type"));
}

/// This test shows how to register connection data for a storage type.
BOOST_AUTO_TEST_CASE(RegisterConnectionDataCreatesConnectionPool) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    NiceMock<MockDriver> mock_driver;
    generateExpectations(mock_driver, 1);

    // Create the database manager with the mock driver.
    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a registration with valid information";
    }

    EXPECT_TRUE(manager.hasConnection("my_storage_type"));
}

/// Requesting a connection will pull one from the connection pool before
/// resorting to creating a new one
BOOST_AUTO_TEST_CASE(CanRequestConnectionAfterRegistering) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    NiceMock<MockDriver> mock_driver;
    generateExpectations(mock_driver, 1);

    // Create the database manager with the mock driver.
    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a registration with valid information";
    }

    EXPECT_TRUE(manager.hasConnection("my_storage_type"));

    auto connection = manager.getConnection("my_storage_type");

    EXPECT_TRUE(connection != nullptr);

    EXPECT_FALSE(manager.hasConnection("my_storage_type"));
}

/// Can request multiple connections
BOOST_AUTO_TEST_CASE(CanRequestMultipleConnections) {    
    // Create a mock driver and set it up to expect a simple successful connection creation.
    NiceMock<MockDriver> mock_driver;
    generateExpectations(mock_driver, 2);

    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a registration with valid information";
    }
    
    std::shared_ptr<sql::Connection> conn1 = manager.getConnection("my_storage_type");
    EXPECT_TRUE(conn1 != nullptr);
    
    std::shared_ptr<sql::Connection> conn2 = manager.getConnection("my_storage_type");
    EXPECT_TRUE(conn2 != nullptr);
}

/// Connections return to the pool when they go out of scope
BOOST_AUTO_TEST_CASE(DeletingConnectionReturnsItToThePool) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    NiceMock<MockDriver> mock_driver;
    generateExpectations(mock_driver, 1);

    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a registration with valid information";
    }
    
    EXPECT_TRUE(manager.hasConnection("my_storage_type"));

    // create a nested scope before requesting the connection, this allows
    // us to test the shared pointer deleter which should add the connection
    // back to the pool if it's still open.
    {
        auto connection = manager.getConnection("my_storage_type");

        EXPECT_TRUE(connection != nullptr);

        EXPECT_FALSE(manager.hasConnection("my_storage_type"));
    }
    
    EXPECT_TRUE(manager.hasConnection("my_storage_type"));
}

BOOST_AUTO_TEST_SUITE_END()
/*****************************************************************************/
// Implementation for the test fixture //

void DatabaseManagerTest::SetUp() {
    host_ = "localhost";
    username_ = "root";
    password_ = "swganh";
}

void DatabaseManagerTest::generateExpectations(NiceMock<MockDriver>& mock_driver, int connections) {
    for (int i=0; i < connections; ++i) {
        MockConnection* connection = new MockConnection();
        
        generateDefaultConnectionExpectations(mock_driver, connection);

        // We should expect a call to the driver's connect for a new connection
        // with these parameters. After returning the expectation should expire.
        EXPECT_CALL(mock_driver, connect(host_, username_, password_))
            .WillOnce(Return(connection))
            .RetiresOnSaturation();
    }
}

void DatabaseManagerTest::generateDefaultConnectionExpectations(NiceMock<MockDriver>& mock_driver, MockConnection* mock_connection) {    
    // Expect the database manager to set the schema.
    EXPECT_CALL(*mock_connection, setSchema(sql::SQLString("galaxy")))
        .Times(1);
    
    EXPECT_CALL(*mock_connection, isClosed())
        .WillRepeatedly(Return(false));

    // Expect isClosed() to return true after a call to close()
    Expectation expect_close1 = EXPECT_CALL(*mock_connection, close());
    EXPECT_CALL(*mock_connection, isClosed())
        .After(expect_close1)
        .WillOnce(Return(true));
}
