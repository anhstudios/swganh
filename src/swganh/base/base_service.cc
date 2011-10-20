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

#include "swganh/base/base_service.h"

#include "anh/app/kernel_interface.h"
#include "anh/database/database_manager_interface.h"
#include "anh/event_dispatcher/event_dispatcher_interface.h"

#include "anh/service/datastore.h"
#include "anh/service/galaxy.h"

#include "swganh/app/swganh_kernel.h"

using namespace anh;
using namespace app;
using namespace swganh::app;
using namespace swganh::base;
using namespace event_dispatcher;
using namespace std;

BaseService::BaseService(shared_ptr<KernelInterface> kernel)
    : kernel_(kernel)
    , active_(kernel->GetIoService()) 
{}

void BaseService::Start() {
    // update the status of the service
    //service_directory_->updateServiceStatus(service_directory()->service(), anh::service::Galaxy::LOADING);
    
    active_.Async([this] () {
        running_ = true;
        
        subscribe();
        
        onStart();
    });
}

void BaseService::Stop() {
    active_.Async([this] () {
        running_ = false;
        
        onStop();
    });
}

bool BaseService::IsRunning() const { return running_; }

shared_ptr<KernelInterface> BaseService::kernel() {
    return kernel_;
}

ActiveObject& BaseService::active() {
    return active_;
}