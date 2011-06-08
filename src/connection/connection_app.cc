/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "connection_app.h"
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_manager.h>
#include <anh/clock.h>
// TEST
#include <packets/Login/LoginClientId.h>
#include <packets/RemoteMessage.h>
#include <packets/NetworkEventMessage.h>

#include <iostream>
#include <glog/logging.h>

#include <boost/thread/thread.hpp>

using namespace std;
using namespace anh;
using namespace scripting;
using namespace component;
using namespace event_dispatcher;
using namespace module_manager;

namespace connection {
ConnectionApp::ConnectionApp(int argc, char* argv[], list<string> config_files
    , shared_ptr<anh::module_manager::PlatformServices> platform_services)
    : BaseApplication(argc, argv, config_files, platform_services) 
{
    event_map_.insert(make_pair(packets::NetworkEventMessage::hash_type(), packets::NetworkEventMessage()));

    auto startupListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Connection Application Startup" <<endl;
        
        cout << "Connection Application Started" << endl;
        started_ = true;
        return true;
    };
    auto processListener = [] (shared_ptr<EventInterface> incoming_event)-> bool {
        return true;
    };
    auto shutdownListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Connection Application Shutdown" <<endl;
        started_ = false;
        return true;
    };
    // setup listener
    auto process_event_listener = [=] (shared_ptr<EventInterface> incoming_event)->bool {
        auto process_event = static_pointer_cast<BasicEvent<packets::RemoteMessage>>(incoming_event);
        // get the proper data struct
        auto data_struct = event_map_[process_event->source_id];
        // fill it out
        data_struct.deserialize(process_event->data);
        // trigger event
        event_dispatcher_->trigger(make_shared_event(data_struct.hash_type(), std::move(data_struct)));
        return true;
    };
    
    event_dispatcher_->subscribe("RemoteMessage", process_event_listener);

    auto debug_event_listener = [=] (shared_ptr<EventInterface> incoming_event)->bool {
        auto debug_event = static_pointer_cast<BasicEvent<packets::NetworkEventMessage>>(incoming_event);
        cout << debug_event->hash_type().ident_string();
        cout << "priority:" << debug_event->priority_id << endl;
        cout << "process_id: " << debug_event->process_id << endl;
        cout << "timestamp: " << debug_event->time_stamp_id << endl;
        return true;
    };
    event_dispatcher_->subscribe("NetworkEventMessage", debug_event_listener);
    // event subscription
    event_dispatcher_->subscribe("Startup", startupListener);
    event_dispatcher_->subscribe("Process", processListener);
    event_dispatcher_->subscribe("Shutdown", shutdownListener);
}
ConnectionApp::~ConnectionApp() {
    //destructor
}
void ConnectionApp::startup() {
    BaseApplication::startup();
    /*auto process = std::make_shared<anh::server_directory::Process>(1,
            1, "ANH.Login","Login","1.0","127.0.0.1",44555,0,0);
    cluster_service_->Connect(process);
    auto test_packet = std::make_shared<packets::LoginClientIdEvent>();
    test_packet->username = "Test";
    test_packet->password = "test";
    test_packet->client_version = "1.0";
    cluster_service_->sendMessage("ANH.Login", test_packet);*/
}
void ConnectionApp::process() {
    BaseApplication::process();
}
void ConnectionApp::shutdown() {
    BaseApplication::shutdown();
}

void ConnectionApp::onAddDefaultOptions_() {
}
void ConnectionApp::onRegisterApp_() {
}


} //namespace connection