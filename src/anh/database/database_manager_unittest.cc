// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

#include "swganh/database/mock_cppconn.h"

#include "swganh/database/database_manager.h"

using namespace swganh::database; 

/// Test fixture for testing the DatabaseManager, provides setup/teardown
class DatabaseManagerTest {
protected:
    void SetUp();
    // Generates expectations for a driver that will return N connections.
    void generateExpectations(MockDriver& mock_driver, int connections);
    void generateDefaultConnectionExpectations(MockDriver& mock_driver, MockConnection* mock_connection);
    void generateExpectationsMultipleConnections(MockDriver& mock_driver);
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
    BOOST_CHECK_NE(manager.hasStorageType("my_storage_type"), true);
}

/// This test shows that a newly created DatabaseManager has no connections
/// registered by default.
BOOST_AUTO_TEST_CASE(NoConnectionsByDefault) {
    MockDriver mock_driver;
    DatabaseManager manager(&mock_driver);
    
    BOOST_CHECK_NE(manager.hasStorageType("my_storage_type"), true);
}

/// This test shows how to register connection data for a storage type.
BOOST_AUTO_TEST_CASE(CanRegisterConnectionDataForStorageType) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    MockDriver mock_driver;
    generateExpectations(mock_driver, 1);

    // Create the database manager with the mock driver.
    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        BOOST_FAIL("No exceptions should be thrown during a registration with valid information");
    }
    BOOST_CHECK(manager.hasStorageType("my_storage_type"));
}

/// This test shows how to register connection data for a storage type.
BOOST_AUTO_TEST_CASE(RegisterConnectionDataCreatesConnectionPool) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    MockDriver mock_driver;
    generateExpectations(mock_driver, 1);

    // Create the database manager with the mock driver.
    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        BOOST_FAIL("No exceptions should be thrown during a registration with valid information");
    }

    BOOST_CHECK(manager.hasStorageType("my_storage_type"));
}

/// Requesting a connection will pull one from the connection pool before
/// resorting to creating a new one
BOOST_AUTO_TEST_CASE(CanRequestConnectionAfterRegistering) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    MockDriver mock_driver;
    MockConnection* mock_connection = new MockConnection();
        
    // Expect the database manager to set the schema.
    MOCK_EXPECT(mock_connection->setSchema)
        .with(sql::SQLString("galaxy"))
        .once();

    mock::sequence s;
    MOCK_EXPECT(mock_connection->isClosed)
        .once()
        .in(s)
        .returns(false);
    
    MOCK_EXPECT(mock_connection->isClosed)
        .once()
        .in(s)
        .returns(true);

    // We should expect a call to the driver's connect for a new connection
    // with these parameters. After returning the expectation should expire.
    MOCK_EXPECT(mock_driver.connect3)
        .once()
        .returns(mock_connection);
    // Create the database manager with the mock driver.
    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        BOOST_FAIL("No exceptions should be thrown during a registration with valid information");
    }

    BOOST_CHECK(manager.hasStorageType("my_storage_type"));

    auto connection = manager.getConnection("my_storage_type");
    
    BOOST_CHECK(connection != nullptr);

    BOOST_CHECK(manager.hasStorageType("my_storage_type"));
}

/// Can request multiple connections
//BOOST_AUTO_TEST_CASE(CanRequestMultipleConnections) {    
//    // Create a mock driver and set it up to expect a simple successful connection creation.
//    MockDriver mock_driver;
//    generateExpectationsMultipleConnections(mock_driver);
//    // Create the database manager with the mock driver.
//    DatabaseManager manager(&mock_driver);
//    
//    // The mysql library relies on 
//    try {
//        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
//    } catch(...) {
//        BOOST_FAIL("No exceptions should be thrown during a registration with valid information");
//    }
//
//    std::shared_ptr<sql::Connection> conn1 = manager.getConnection("my_storage_type");
//    
//    BOOST_CHECK(conn1 != nullptr);
//    
//    std::shared_ptr<sql::Connection> conn2 = manager.getConnection("my_storage_type");
//    
//    BOOST_CHECK(conn2 != nullptr);
//
//}

/// Connections return to the pool when they go out of scope
BOOST_AUTO_TEST_CASE(DeletingConnectionReturnsItToThePool) {
    // Create a mock driver and set it up to expect a simple successful connection creation.
    MockDriver mock_driver;
    MockConnection* mock_connection = new MockConnection();
        
    // Expect the database manager to set the schema.
    MOCK_EXPECT(mock_connection->setSchema)
        .with(sql::SQLString("galaxy"))
        .once();

    mock::sequence s;
    MOCK_EXPECT(mock_connection->isClosed)
        .once()
        .in(s)
        .returns(false);
    
    MOCK_EXPECT(mock_connection->isClosed)
        .once()
        .in(s)
        .returns(true);

    // We should expect a call to the driver's connect for a new connection
    // with these parameters. After returning the expectation should expire.
    MOCK_EXPECT(mock_driver.connect3)
        .once()
        .returns(mock_connection);
    // Create the database manager with the mock driver.
    DatabaseManager manager(&mock_driver);
    
    // The mysql library relies on 
    try {
        manager.registerStorageType("my_storage_type", "galaxy", host_, username_, password_);
    } catch(...) {
        BOOST_FAIL("No exceptions should be thrown during a registration with valid information");
    }
    
    BOOST_CHECK(manager.hasStorageType("my_storage_type"));

    // create a nested scope before requesting the connection, this allows
    // us to test the shared pointer deleter which should add the connection
    // back to the pool if it's still open.
    {
        auto connection = manager.getConnection("my_storage_type");

        BOOST_CHECK(connection != nullptr);

        BOOST_CHECK(manager.hasStorageType("my_storage_type"));
    }
    
    BOOST_CHECK(manager.hasStorageType("my_storage_type"));
}

BOOST_AUTO_TEST_SUITE_END()
/*****************************************************************************/
// Implementation for the test fixture //

void DatabaseManagerTest::SetUp() {
    host_ = "localhost";
    username_ = "root";
    password_ = "swganh";
}
void DatabaseManagerTest::generateExpectationsMultipleConnections(MockDriver& mock_driver) {
    MockConnection mock_connection;
    
    // We should expect a call to the driver's connect for a new connection
    // with these parameters. After returning the expectation should expire.
    MOCK_EXPECT(mock_driver.connect3)
        .once()
        .returns(&mock_connection);

    
    // Expect the database manager to set the schema.
    MOCK_EXPECT(mock_connection.setSchema)
        .with(sql::SQLString("galaxy"))
        .once();

    MOCK_EXPECT(mock_connection.isClosed)
        .exactly(2)
        .returns(false);
    // We should expect a call to the driver's connect for a new connection
    // with these parameters. After returning the expectation should expire.
    MOCK_EXPECT(mock_driver.connect3)
        .once()
        .returns(&mock_connection);

    // Expect the database manager to set the schema.
    MOCK_EXPECT(mock_connection.setSchema)
        .with(sql::SQLString("galaxy"))
        .once();

    mock::sequence s;

    // Expect isClosed() to return true after a call to close()
    
    /*MOCK_EXPECT(*mock_connection, close)
        .once()
        .in(s);*/
        

    MOCK_EXPECT(mock_connection.isClosed)
        .once()
        .in(s)
        .returns(true);

}

void DatabaseManagerTest::generateExpectations(MockDriver& mock_driver, int connections) {
    for (int i=0; i < connections; ++i) {
        MockConnection* connection = new MockConnection();
        
        generateDefaultConnectionExpectations(mock_driver, connection);

        // We should expect a call to the driver's connect for a new connection
        // with these parameters. After returning the expectation should expire.
        MOCK_EXPECT(mock_driver.connect3)
            .once()
            .returns(connection);
    }
}

void DatabaseManagerTest::generateDefaultConnectionExpectations(MockDriver& mock_driver, MockConnection* mock_connection) {    
    // Expect the database manager to set the schema.
    MOCK_EXPECT(mock_connection->setSchema)
        .with(sql::SQLString("galaxy"))
        .once();
    mock::sequence s;

    // Expect isClosed() to return true after a call to close()
    
    MOCK_EXPECT(mock_connection->close)
        .once()
        .in(s);
        

    MOCK_EXPECT(mock_connection->isClosed)
        .once()
        .in(s)
        .returns(true);

}
