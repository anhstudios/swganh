/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team */

#include <login/login_app.h>
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_manager.h>
#include <anh/clock.h>

#include <iostream>
#include <glog/logging.h>
#include <boost/thread/thread.hpp>

using namespace std;
using namespace anh;
using namespace scripting;
using namespace event_dispatcher;
using namespace module_manager;
using namespace login;
int main(int argc, char* argv[])
{
    /// pre startup
    // Initialize the google logging.
    google::InitGoogleLogging(argv[0]);

    #ifndef _WIN32
        google::InstallFailureSignalHandler();
    #endif

    FLAGS_log_dir = "./logs";
    FLAGS_stderrthreshold = 1;
    setvbuf( stdout, NULL, _IONBF, 0);
    // config files
    list<string> config;
    config.push_back("config/login.cfg");
    shared_ptr<EventDispatcherInterface> dispatcher = make_shared<EventDispatcher>();
    shared_ptr<Clock> clock = make_shared<Clock>();
    auto services = make_shared<PlatformServices>();
    // add services
    services->addService("EventDispatcher", dispatcher);
    services->addService("Clock", clock);
    LoginApp app(argc, argv, config, services);
    ///
    app.startup();
    boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    if (true) {
        while(true){
            app.process();
            if (app.kbHit())
            {
                if(cin.get() == 'q')
                {
                    app.shutdown();
                    return 0;
                }
                else
                {
                    // add in test for tcp connections
                    std::string input;
                    getline(cin, input);
                    if (strcmp("tcp send", input.c_str()))
                    {
                        istringstream iss(input);
                        std::vector<string> inputs;
                        do {
                            string sub;
                            iss >> sub;
                            inputs.push_back(sub);
                        } while (iss);
                        inputs.push_back("/r/n");
                        uint16_t port = boost::lexical_cast<uint16_t>(inputs[2]);
                        string message;
                        std::for_each(inputs.begin()+3, inputs.end(), [&message] (string str){
                            message += str;
                            message += " ";
                        });
                        cout << "message: " << message;
                        cout << port << inputs[2];
                        app.cluster_service()->sendMessage("127.0.0.1", port, anh::ByteBuffer((const unsigned char*)message.c_str(), message.size()));
                    }

                }
            }
            boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        }
    }
}