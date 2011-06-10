/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team*/

#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <anh/application.h>
#include <anh/database/mock_cppconn.h>
#include <anh/database/mock_database_manager.h>
#include <anh/scripting/mock_scripting_manager.h>
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/server_directory/mock_server_directory.h>
#include <anh/event_dispatcher/mock_event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/network/cluster/mock_service.h>
#include <anh/module_manager/module_manager.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/mock_platform_services.h>
#include <anh/network/cluster/cluster_service.h>
#include <anh/clock.h>

using namespace std;
using namespace anh;
using namespace testing;
using namespace event_dispatcher;
using namespace database;
using namespace scripting;
using namespace server_directory;
using namespace module_manager;
using namespace network::cluster;
using testing::DefaultValue;

namespace {

class MockApplication : public BaseApplication {
public:
    MockApplication(list<string> config_files
    , shared_ptr<PlatformServices> services)
    : BaseApplication(config_files, services){};
    MOCK_CONST_METHOD0(hasStarted, bool());
    MOCK_METHOD0(onAddDefaultOptions_, void());
    MOCK_METHOD0(onRegisterApp_, void());
    MOCK_METHOD0(setupCluster_, void());
};

class ApplicationTest : public testing::Test
{
public:
    list<string> config;
    shared_ptr<MockScriptingManager> scripter;
    shared_ptr<NiceMock<MockDatabaseManager>> manager;
    shared_ptr<NiceMock<MockServerDirectory>> directory;
    shared_ptr<NiceMock<MockEventDispatcher>> mock_dispatcher;  
    shared_ptr<NiceMock<ClusterService>>clust_service;
    shared_ptr<ModuleManager> module_manager;
    shared_ptr<PlatformServices> services;

    shared_ptr<MockApplication> buildBasicApplication() {
        shared_ptr<ScriptingManagerInterface> scripting_mgr = make_shared<ScriptingManager>("scripts");
        shared_ptr<EventDispatcherInterface> event_dispatcher = make_shared<NiceMock<MockEventDispatcher>>();
        shared_ptr<DatabaseManagerInterface> db_manager = make_shared<NiceMock<MockDatabaseManager>>();
        shared_ptr<ServerDirectoryInterface> server_directory = make_shared<NiceMock<MockServerDirectory>>();
        shared_ptr<ClusterServiceInterface> cluster_service = make_shared<NiceMock<MockClusterService>>();
        // goofy way of doing this, but since we're using mocks it's necessary.
        mock_dispatcher = static_pointer_cast<NiceMock<MockEventDispatcher>>(event_dispatcher);
        scripter = static_pointer_cast<MockScriptingManager>(scripting_mgr);
        manager = static_pointer_cast<NiceMock<MockDatabaseManager>>(db_manager);
        directory = static_pointer_cast<NiceMock<MockServerDirectory>>(server_directory);
        //clust_service = static_pointer_cast<NiceMock<MockClusterService>>(cluster_service);
        /// end goofy
        shared_ptr<Clock> clock = make_shared<Clock>();
        services = make_shared<PlatformServices>();
        services->addService("ScriptingManager", scripting_mgr);
        services->addService("EventDispatcher", event_dispatcher);
        services->addService("DatabaseManager", db_manager);
        services->addService("ServerDirectory", server_directory);
        services->addService("ClusterService", cluster_service);
        services->addService("Clock", clock);
        
        return make_shared<MockApplication>(config, services);
    }
protected:
    virtual void SetUp();
    virtual void TearDown();
};

/// tests that the app won't do any processing if the startup process hasn't run
TEST_F(ApplicationTest, dieOnProcessIfNotStarted) {
    shared_ptr<MockApplication> app = buildBasicApplication();

    EXPECT_CALL(*app, hasStarted())
        .WillRepeatedly(Return(false));

    ASSERT_DEATH(app->process(), "Must call startup before process");
}
/// verify the Startup event is triggered on startup
TEST_F(ApplicationTest, startupEventTriggered) {
    shared_ptr<MockApplication> app = buildBasicApplication();

    EXPECT_CALL(*mock_dispatcher, trigger(_));
    
    app->startup();

    EXPECT_CALL(*app, hasStarted())
        .WillRepeatedly(Return(true));
}
/// verify the Startup event is not triggered if startup isn't called
TEST_F(ApplicationTest, startupEventNotTriggered) {
    shared_ptr<MockApplication> app = buildBasicApplication();

    EXPECT_CALL(*app, hasStarted())
        .WillRepeatedly(Return(false));
    
    EXPECT_CALL(*mock_dispatcher, trigger(_))
        .Times(0);

    ASSERT_DEATH(app->process(), "Must call startup before process");

    EXPECT_CALL(*app, hasStarted())
        .WillRepeatedly(Return(false));
}

/// verify the Process event triggered 
TEST_F(ApplicationTest, processEventTriggered) {
    shared_ptr<MockApplication> app = buildBasicApplication();

    EXPECT_CALL(*app, hasStarted())
        .WillRepeatedly(Return(false));

    // once for Startup and once for Process
    EXPECT_CALL(*mock_dispatcher, trigger(_))
        .Times(2);

    app->startup();

    EXPECT_CALL(*app, hasStarted())
        .WillRepeatedly(Return(true));

    app->process();
}

/// Verifies that process cannot be called after shutdown.
/// also verifies no events occur after the shutdown
TEST_F(ApplicationTest, DiesWhenProcessCalledAfterShutdown) {
    shared_ptr<MockApplication> app = buildBasicApplication();

    Expectation expect_on_startup = EXPECT_CALL(*mock_dispatcher, trigger(_));
    EXPECT_CALL(*app, hasStarted())
        .After(expect_on_startup)
        .WillRepeatedly(Return(true));
    // start testing the app here
    app->startup();
    
    EXPECT_CALL(*mock_dispatcher, trigger(_));

    ASSERT_TRUE(app->hasStarted());
    app->process();

    Expectation expect_on_shutdown = EXPECT_CALL(*mock_dispatcher, trigger(_));
    EXPECT_CALL(*app, hasStarted())
        .After(expect_on_shutdown)
        .WillRepeatedly(Return(false));

    app->shutdown();

    EXPECT_CALL(*mock_dispatcher, trigger(_))
        .Times(0);

    ASSERT_DEATH(app->process(), "Must call startup before process");
}

/// checks based on a test cfg file we are able to load and register two storage types
TEST_F(ApplicationTest, doesLoadConfigurationFile)
{
    shared_ptr<MockApplication> app = buildBasicApplication();

    EXPECT_CALL(*manager, registerStorageType(_,_,_,_,_))
        .Times(2);
    
    EXPECT_NO_THROW(
        app->startup();    
    );
}

/// checks to see that the virtual function onAddDefaultOptions is called
TEST_F(ApplicationTest, onAddDefaultOptionsCalled)
{
    shared_ptr<MockApplication> app = buildBasicApplication();
    EXPECT_CALL(*app, onAddDefaultOptions_());
    app->startup();
}
// TODO: add back when server directory is fixed
/// checks if on startup the process is registered with server directory
//TEST_F(ApplicationTest, doesRegisterProcess)
//{
//    shared_ptr<MockApplication> app = buildBasicApplication();
//    EXPECT_CALL(*directory, registerProcess(_, _, _, _, 44992, 0, 0));
//
//    EXPECT_NO_THROW(
//        app->startup();
//    );
//}

void ApplicationTest::SetUp()
{    
    // create test files
    ofstream of("general.cfg");
    of << "# Cluster Configuration " << endl;
    of << "cluster.name = naritus " << endl;
    of << "cluster.process_type = test_service" << endl;
    of << "cluster.tcp_port = 44992" << endl;
    of << "cluster.datastore.name = global "<< endl;
    of << "cluster.datastore.host = localhost "<< endl;
    of << "cluster.datastore.username = root " << endl;
    of << "cluster.datastore.password = swganh " << endl;
    of << "cluster.datastore.schema = swganh_process " << endl;
    of << "# Galaxy Configuration "<< endl;
    of << "galaxy.datastore.name = galaxy " << endl;
    of << "galaxy.datastore.host = localhost " << endl;
    of << "galaxy.datastore.username = root " << endl;
    of << "galaxy.datastore.password = swganh " << endl;
    of << "galaxy.datastore.schema = swganh " << endl;
    of << "modules = ./module/anh_mod_transform " << endl;
    of << "modules = ./module/anh_mod_appearance" << endl;
    of.flush();
    of.close();
    of.open("invalid_data.cfg");
    of << "nothing to see here" << endl;
    of.close();

    config.push_back("general.cfg");

     // setup default values;
    auto cluster = std::make_shared<Cluster>(0, 0, "", "", Cluster::ONLINE, "", "");
    ProcessList proc_list;
    DefaultValue<std::shared_ptr<Cluster>>::Set(cluster);
    DefaultValue<ProcessList>::Set(proc_list);
}
void ApplicationTest::TearDown()
{
    remove("general.cfg");
    remove("invalid_data.cfg");
}
}  // namespace