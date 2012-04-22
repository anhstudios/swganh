/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <exception>
#include <iostream>
#include <string>

#include <boost/thread.hpp>

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
            } else if(cmd.compare("console") == 0) {
                anh::Logger::getInstance().DisableConsoleLogging();

                std::system("cls");
                std::cout << "swgpy console " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;

                swganh::scripting::ScopedGilLock lock;
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
