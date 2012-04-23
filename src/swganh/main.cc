// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <exception>
#include <iostream>
#include <string>

#include <boost/thread.hpp>
#include <boost/python.hpp>

#include "anh/logger.h"

#include "swganh/app/swganh_app.h"
#include "swganh/scripting/utilities.h"

#include "version.h"

using namespace boost;
using namespace swganh;
using namespace std;

int main(int argc, char* argv[]) 
{
    Py_Initialize();
	PyEval_InitThreads();
    
    // Step 2: Release the GIL from the main thread so that other threads can use it
    PyEval_ReleaseThread(PyGILState_GetThisThreadState());
    
    try {
        app::SwganhApp app;

        app.Initialize(argc, argv);

        app.Start();

        for (;;) {
            string cmd;
            cin >> cmd;

            if (cmd.compare("exit") == 0 || cmd.compare("quit") == 0 || cmd.compare("q") == 0) {
                LOG(info) << "Exit command received from command line. Shutting down.";
                
                // Stop the application and join the thread until it's finished.
                app.Stop();
				
                break;
            } else if(cmd.compare("console") == 0 || cmd.compare("~") == 0) {
                swganh::scripting::ScopedGilLock lock;
                anh::Logger::getInstance().DisableConsoleLogging();

#ifdef WIN32
                std::system("cls");
#else
                if (std::system("clear") != 0)
                {
                    LOG(error) << "Error clearing screen, ignoring console mode";
                    continue;
                }
#endif
                std::cout << "swgpy console " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;

                boost::python::object main = boost::python::object (boost::python::handle<>(boost::python::borrowed(
                    PyImport_AddModule("__main__")
                )));
                auto global_dict = main.attr("__dict__");
                global_dict["kernel"] = boost::python::ptr(app.GetAppKernel());

                PyRun_InteractiveLoop(stdin, "<stdin>");

                anh::Logger::getInstance().EnableConsoleLogging();
            } else {
                LOG(warning) << "Invalid command received: " << cmd;
                std::cout << "Type exit or (q)uit to quit" << std::endl;
            }
        }

    } catch(std::exception& e) {
        LOG(fatal) << "Unhandled application exception occurred: " << e.what();
    }

    // Step 4: Lock the GIL before calling finalize
    PyGILState_Ensure();
    Py_Finalize();

    return 0;
}
