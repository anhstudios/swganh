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

#include "swganh/app/swganh_app.h"

#include <exception>
#include <iostream>
#include <string>

#include "anh/logger.h"
#include <boost/thread.hpp>

using namespace boost;
using namespace swganh;
using namespace std;

int main(int argc, char* argv[]) 
{
    try {
        app::SwganhApp app;

        app.Initialize(argc, argv);

        boost::thread application_thread([&app] () {
            app.Start();
        });

        for (;;) {
            string cmd;
            cin >> cmd;

            if (cmd.compare("exit") == 0 || cmd.compare("quit") == 0 || cmd.compare("q") == 0) {
                LOG(info) << "Exit command received from command line. Shutting down.";
                
                // Stop the application and join the thread until it's finished.
                app.Stop();
                if (application_thread.joinable())
					application_thread.join();
				else
					application_thread.interrupt();
				
                break;
            } else {
                LOG(warning) << "Invalid command received: " << cmd;
            }
        }

    } catch(std::exception& e) {
        LOG(fatal) << "Unhandled application exception occurred: " << e.what();
    }

    return 0;
}
