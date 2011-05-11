/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifdef ERROR
#undef ERROR
#endif

#include "login_app.h"
#include <anh/event_dispatcher/event_dispatcher.h>
#include <anh/scripting/scripting_manager.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_manager.h>
#include <anh/clock.h>

#ifdef ERROR
#undef ERROR
#endif

#include <iostream>
#include <glog/logging.h>
#include <boost/thread/thread.hpp>

using namespace std;
using namespace anh;
using namespace scripting;
using namespace event_dispatcher;
using namespace module_manager;
using namespace boost::program_options;

namespace login {
LoginApp::LoginApp(int argc, char* argv[], list<string> config_files
    , shared_ptr<anh::module_manager::PlatformServices> platform_services)
    : BaseApplication(argc, argv, config_files, platform_services) 
	, soe_service_()
{
    auto startupListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Login Application Startup" <<endl;
        started_ = true;
        cout << "Login Application Started" << endl;

        return true;
    };
    auto processListener = [] (shared_ptr<EventInterface> incoming_event)-> bool {
        return true;
    };
    auto shutdownListener = [&] (shared_ptr<EventInterface> incoming_event)-> bool {
        cout << "Login Application Shutdown" <<endl;
        started_ = false;
        return true;
    };

    // event subscription
    event_dispatcher_->subscribe("Startup", startupListener);
    event_dispatcher_->subscribe("Process", processListener);
    event_dispatcher_->subscribe("Shutdown", shutdownListener);
}
LoginApp::~LoginApp() {
    //destructor
}

void LoginApp::startup()
{
	soe_service_.Start(configuration_variables_map_["soe_service.port"].as<uint16_t>());
}

void LoginApp::process()
{
	soe_service_.Update();
}

void LoginApp::shutdown()
{
	soe_service_.Shutdown();
}

void LoginApp::onAddDefaultOptions_() 
{
	configuration_options_description_.add_options()
		("soe_service.port", value<uint16_t>()->default_value(44453), "Port to run the SOE Frontend Service on.")
		;
}

void LoginApp::onRegisterApp_() 
{
}
} //namespace login
