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

#include "anh/service_directory/galaxy.h"
#include "anh/service_directory/datastore.h"
#include "anh/service_directory/service.h"
#include "anh/service_directory/service_directory.h"
#include <anh/event_dispatcher/event_dispatcher.h>

using namespace anh::service_directory;
using namespace anh::event_dispatcher;
using namespace std;
using namespace boost::posix_time;
using namespace testing;


// Test harness for the ServiceDirectory unit tests, provides a database_connection_
// to use for testing.
class ServiceDirectoryTest : public testing::Test {
protected:
    virtual void SetUp() {
        test_galaxy_ = make_shared<Galaxy>(getTestGalaxy());
        test_service_ = make_shared<Service>(getTestService());
        dispatcher_ = make_shared<EventDispatcher>();
    }

    Galaxy getTestGalaxy() {
        Galaxy galaxy(1, 2, "test_galaxy", "20050408-18:00", Galaxy::OFFLINE, "1970-01-01 00:00:01", "1970-01-01 00:00:01");
        return galaxy;
    }
    
    Service getTestService() {
        Service service(1, 1, "service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0);
        return service;
    }

    std::shared_ptr<Galaxy> test_galaxy_;
    std::shared_ptr<Service> test_service_;
    std::shared_ptr<EventDispatcher> dispatcher_;
};

class MockDatastore : public DatastoreInterface {
public:
    MOCK_CONST_METHOD1(findGalaxyByName, shared_ptr<Galaxy>(const std::string& name));
    MOCK_CONST_METHOD2(createGalaxy, shared_ptr<Galaxy>(const std::string& name, const std::string& version));
    MOCK_CONST_METHOD8(createService, shared_ptr<Service>(std::shared_ptr<Galaxy> galaxy, const std::string& name, const std::string& type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port, uint16_t ping_port));
    MOCK_CONST_METHOD1(getGalaxyTimestamp, std::string(std::shared_ptr<Galaxy> galaxy));
    MOCK_CONST_METHOD1(saveService, void(std::shared_ptr<Service> service));
    MOCK_CONST_METHOD1(findGalaxyById, shared_ptr<Galaxy>(uint32_t id));
    MOCK_CONST_METHOD1(deleteServiceById, bool(uint32_t id));
    MOCK_CONST_METHOD0(getGalaxyList, GalaxyList());
    MOCK_CONST_METHOD1(getServiceList, ServiceList(uint32_t galaxy_id));
    MOCK_CONST_METHOD1(prepareTimestampForStorage, std::string(const std::string& timestamp));
};

/// Creating and using an instance of ServiceDirectory requires a valid galaxy
/// so the service of constructing should join with a galaxy.
TEST_F(ServiceDirectoryTest, CreatingServiceDirectoryJoinsToGalaxy) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(test_galaxy_));
        
    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "test_galaxy", "20050408-18:00");
    
        auto galaxy = service_directory.galaxy();

        EXPECT_EQ("test_galaxy", galaxy->name());
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful join";
    }
}

/// When creating an instance of ServiceDirectory with an invalid galaxy name
/// an InvalidGalaxy exception is thrown.
TEST_F(ServiceDirectoryTest, JoiningInvalidGalaxyThrowsException) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("bad_galaxy"))
        .WillOnce(Return(nullptr));

    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "bad_galaxy", "20050408-18:00");
        FAIL() << "An exception should be thrown when given an invalid galaxy name";
    } catch(const InvalidGalaxyError& /*e*/) {
        SUCCEED();
    }
}

/// Can join after creating service directory instance.
TEST_F(ServiceDirectoryTest, CanJoinGalaxyAfterCreation) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(test_galaxy_));
        
    try {
        ServiceDirectory service_directory(datastore, dispatcher_);

        service_directory.joinGalaxy("test_galaxy", "20050408-18:00");
    
        auto galaxy = service_directory.galaxy();

        EXPECT_EQ("test_galaxy", galaxy->name());
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful join";
    }
}


/// Passing an optional parameter to the constructor while create a galaxy
/// and set it as the active galaxy.
TEST_F(ServiceDirectoryTest, CanCreateGalaxyWhenJoining) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*datastore, createGalaxy("test_galaxy", "20050408-18:00"))
        .WillOnce(Return(test_galaxy_));
    
    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "test_galaxy", "20050408-18:00", true);
        
        auto galaxy = service_directory.galaxy();

        EXPECT_EQ("test_galaxy", galaxy->name());
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

/// Registering a service makes it the active service.
TEST_F(ServiceDirectoryTest, RegisteringServiceMakesItActive) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(test_galaxy_));

    EXPECT_CALL(*datastore, createService(_, "service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_service_));

    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "test_galaxy", "20050408-18:00");

        EXPECT_TRUE(service_directory.registerService("service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0));

        auto service = service_directory.service();
        
        EXPECT_EQ("service_name", service->name());
        EXPECT_EQ("test_service", service->type());
    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

TEST_F(ServiceDirectoryTest, CanMakeActiveServiceThePrimaryGalaxyService) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(test_galaxy_));

    EXPECT_CALL(*datastore, createService(_, "service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_service_));

    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "test_galaxy", "20050408-18:00");

        EXPECT_TRUE(service_directory.registerService("service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0));
        
        auto galaxy = service_directory.galaxy();
        auto service = service_directory.service();

        EXPECT_NE(galaxy->primary_id(), service->id());

        EXPECT_TRUE(service_directory.makePrimaryService(service));
        
        galaxy = service_directory.galaxy();
        service = service_directory.service();

        EXPECT_EQ(galaxy->primary_id(), service->id());

    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

TEST_F(ServiceDirectoryTest, PulsingUpdatesActiveServiceTimestamp) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(test_galaxy_));

    EXPECT_CALL(*datastore, createService(_, "service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_service_));

    EXPECT_CALL(*datastore, getGalaxyTimestamp(_))
        .WillOnce(Return(std::string("1970-01-01 00:01:01")));

    EXPECT_CALL(*datastore, saveService(test_service_))
        .Times(1);
    
    EXPECT_CALL(*datastore, findGalaxyById(test_galaxy_->id()))
        .WillOnce(Return(test_galaxy_));

    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "test_galaxy", "20050408-18:00");

        EXPECT_TRUE(service_directory.registerService("service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0));
        
        auto service = service_directory.service();
        std::string registration_time = service->last_pulse();
        
        service_directory.pulse();
                
        service = service_directory.service();
        std::string pulse_time = service->last_pulse();
        
        EXPECT_NE(registration_time, pulse_time);
        EXPECT_LT(ptime(time_from_string(registration_time)),
                  ptime(time_from_string(pulse_time)));
    } catch(const std::exception& e) {
        FAIL() << "No exceptions should be thrown during a successful create/join" << e.what();
    }
}

/// When deleting a service it should be nullified
TEST_F(ServiceDirectoryTest, RemovingServiceNullifiesIt) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(test_galaxy_));

    EXPECT_CALL(*datastore, createService(_, "service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_service_));
    
    EXPECT_CALL(*datastore, deleteServiceById(1))
        .WillOnce(Return(true));

    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "test_galaxy", "20050408-18:00");

        EXPECT_TRUE(service_directory.registerService("service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0));

        auto service = service_directory.service();
        
        EXPECT_EQ("service_name", service->name());
        EXPECT_EQ("test_service", service->type());

        service_directory.removeService(service);

        EXPECT_EQ(nullptr, service);

    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}

/// When deleting a service it should be nullified, if it's the active service
/// then the instance held by ServiceDirectory needs to be updated as well.
TEST_F(ServiceDirectoryTest, RemovingActiveServiceNullifiesIt) {
    auto datastore = make_shared<MockDatastore>();
    EXPECT_CALL(*datastore, findGalaxyByName("test_galaxy"))
        .WillOnce(Return(test_galaxy_));

    EXPECT_CALL(*datastore, createService(_, "service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0))
        .WillOnce(Return(test_service_));

    EXPECT_CALL(*datastore, deleteServiceById(1))
        .WillOnce(Return(true));

    try {
        ServiceDirectory service_directory(datastore, dispatcher_, "test_galaxy", "20050408-18:00");

        EXPECT_TRUE(service_directory.registerService("service_name", "test_service", "1.0.0", "127.0.0.1", 0, 40000, 0));

        auto service = service_directory.service();
        
        EXPECT_EQ("service_name", service->name());
        EXPECT_EQ("test_service", service->type());

        service_directory.removeService(service);

        EXPECT_EQ(nullptr, service_directory.service());

    } catch(...) {
        FAIL() << "No exceptions should be thrown during a successful create/join";
    }
}
