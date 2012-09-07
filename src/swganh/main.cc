// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include <exception>
#include <iostream>
#include <string>

#include <boost/thread.hpp>
#include <boost/python.hpp>

#include "swganh/logger.h"
#include "swganh/utilities.h"

#include "swganh/app/swganh_app.h"
#include "swganh/scripting/utilities.h"

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
        app::SwganhApp app(argc, argv);

        for (;;) {

            if (swganh::KeyboardHit())
            {
                char input = swganh::GetHitKey();
                if (input == '`' || input =='~')
                {
                    app.StartInteractiveConsole();
                }
                else
                {   
                    // Echo out the input that was given and add it back to the input stream to read in.
                    std::cout << input;
                    cin.putback(input);

                    string cmd;
                    cin >> cmd;

                    if (cmd.compare("exit") == 0 || cmd.compare("quit") == 0 || cmd.compare("q") == 0) {
                        LOG(info) << "Exit command received from command line. Shutting down.";
        
                        break;
                    } else {
                        LOG(warning) << "Invalid command received: " << cmd;
                        std::cout << "Type exit or (q)uit to quit" << std::endl;
                    }
                }
            }
			boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        }

    } catch(std::exception& e) {
        LOG(fatal) << "Unhandled application exception occurred: " << e.what();
    }

    // Step 4: Lock the GIL before calling finalize
    PyGILState_Ensure();
    Py_Finalize();

    return 0;
}
