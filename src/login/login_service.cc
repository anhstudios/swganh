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

#include "login/login_service.h"

using namespace anh::event_dispatcher;
using namespace login;
using namespace std;

LoginService::LoginService(shared_ptr<EventDispatcherInterface> event_dispatcher)
    : soe_service_()
    , event_dispatcher_(event_dispatcher)
    , running_(false)
{
   soe_service_.event_dispatcher(event_dispatcher_);
}

LoginService::~LoginService() {}

void LoginService::Run() {
    running_ = true;

    soe_service_.Start(44453);

    while(IsRunning()) {
        soe_service_.Update();
    }

    soe_service_.Shutdown();
}

bool LoginService::IsRunning() const {
    return running_;
}
