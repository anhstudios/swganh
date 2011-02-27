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
#include <anh/server_directory/mock_server_directory.h>
#include <anh/event_dispatcher/mock_event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>

using namespace std;
using namespace anh;
using namespace testing;
using namespace event_dispatcher;
using namespace database;
using namespace scripting;
using namespace server_directory;

namespace {

class MockApplication : public BaseApplication {
public:
    MockApplication(list<string> config_files, shared_ptr<EventDispatcherInterface> event_dispatcher
    , shared_ptr<DatabaseManagerInterface> db_manager, shared_ptr<ScriptingManagerInterface> scripting_manager
    , shared_ptr<ServerDirectoryInterface> server_directory)
    : BaseApplication(config_files, event_dispatcher, db_manager, scripting_manager, server_directory){};
    MOCK_CONST_METHOD0(hasStarted, bool());
    MOCK_METHOD0(onAddDefaultOptions_, void());
    MOCK_METHOD0(onRegisterApp_, void());
};

class ApplicationTest : public testing::Test
{
public:
    list<string> config;
    shared_ptr<MockScriptingManager> scripter;
    shared_ptr<NiceMock<MockDatabaseManager>> manager;
    shared_ptr<NiceMock<MockServerDirectory>> directory;
    shared_ptr<NiceMock<MockEventDispatcher>> mock_dispatcher;  

    shared_ptr<MockApplication> buildBasicApplication() {
        scripter = make_shared<MockScriptingManager>();
        manager = make_shared<NiceMock<MockDatabaseManager>>();
        directory = make_shared<NiceMock<MockServerDirectory>>();
        mock_dispatcher = make_shared<NiceMock<MockEventDispatcher>>();
        return make_shared<MockApplication>(config, mock_dispatcher, manager, scripter, directory);
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

    EXPECT_CALL(*manager, registerStorageType(_,"swganh_static","localhost","root", "swganh"));
    EXPECT_CALL(*manager, registerStorageType(_,"swganh","localhost","root", "swganh"));
    
    EXPECT_NO_THROW(
        app->startup();    
    );
}
TEST_F(ApplicationTest, cantLoadConfigFile)
{
    scripter = make_shared<MockScriptingManager>();
    manager = make_shared<NiceMock<MockDatabaseManager>>();
    directory = make_shared<NiceMock<MockServerDirectory>>();
    mock_dispatcher = make_shared<NiceMock<MockEventDispatcher>>();
    list<string> config_list;
    config_list.push_back("notfound.cfg");
    shared_ptr<MockApplication> app = make_shared<MockApplication>(config_list, mock_dispatcher, manager, scripter, directory);
    // expectation is an exception is thrown as file not found
    EXPECT_ANY_THROW(
       app->startup();
    ); 
}
/// checks based on a test cfg file we are able to load and register two storage types
TEST_F(ApplicationTest, foundConfigNoValidValues)
{
    scripter = make_shared<MockScriptingManager>();
    manager = make_shared<NiceMock<MockDatabaseManager>>();
    directory = make_shared<NiceMock<MockServerDirectory>>();
    mock_dispatcher = make_shared<NiceMock<MockEventDispatcher>>();
    list<string> config_list;
    config_list.push_back("invalid_data.cfg");
    shared_ptr<MockApplication> app = make_shared<MockApplication>(config_list, mock_dispatcher, manager, scripter, directory);
    
    EXPECT_ANY_THROW(
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
/// checks if on startup the process is registered with server directory
TEST_F(ApplicationTest, doesRegisterProcess)
{
    shared_ptr<BaseApplication> app = buildBasicApplication();
    EXPECT_CALL(*directory, registerProcess(_, _, _, _, 44992, 0, 0));

    EXPECT_NO_THROW(
        app->startup();
    );
}
TEST_F(ApplicationTest, doesHandleNullPtrScripter)
{
    scripter = nullptr;
    manager = make_shared<NiceMock<MockDatabaseManager>>();
    directory = make_shared<NiceMock<MockServerDirectory>>();
    mock_dispatcher = make_shared<NiceMock<MockEventDispatcher>>();

    EXPECT_NO_THROW(
        shared_ptr<MockApplication> app = make_shared<MockApplication>(config, mock_dispatcher, manager, scripter, directory);
        );
}
TEST_F(ApplicationTest, doesHandleNullPtrEvent)
{
    scripter = nullptr;
    manager = make_shared<NiceMock<MockDatabaseManager>>();
    directory = make_shared<NiceMock<MockServerDirectory>>();
    mock_dispatcher = nullptr;
    // event dispatcher shouldn't throw an exception
    shared_ptr<MockApplication> app = make_shared<MockApplication>(config, mock_dispatcher, manager, scripter, directory);
    EXPECT_NO_THROW(
        app->startup();    
        );
}
TEST_F(ApplicationTest, doesHandleNullPtrDB)
{
    config.push_back("general.cfg");

    scripter = nullptr;
    manager = nullptr;
    directory = make_shared<NiceMock<MockServerDirectory>>();
    mock_dispatcher = make_shared<NiceMock<MockEventDispatcher>>();
    // we'll build this up if they don't send one
    EXPECT_NO_THROW(
        shared_ptr<MockApplication> app = make_shared<MockApplication>(config, mock_dispatcher, manager, scripter, directory);
        );
}

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
    of << "cluster.datastore.schema = swganh_static " << endl;
    of << "# Galaxy Configuration "<< endl;
    of << "galaxy.datastore.name = galaxy " << endl;
    of << "galaxy.datastore.host = localhost " << endl;
    of << "galaxy.datastore.username = root " << endl;
    of << "galaxy.datastore.password = swganh " << endl;
    of << "galaxy.datastore.schema = swganh " << endl;
    of.flush();
    of.close();
    of.open("invalid_data.cfg");
    of << "nothing to see here" << endl;
    of.close();

    config.push_back("general.cfg");
}
void ApplicationTest::TearDown()
{
    remove("general.cfg");
    remove("invalid_data.cfg");
}
}  // namespace