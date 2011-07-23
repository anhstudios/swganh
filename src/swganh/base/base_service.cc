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

#include "swganh/app/swganh_kernel.h"

using namespace anh;
using namespace app;
using namespace swganh::app;
using namespace swganh::base;
using namespace event_dispatcher;
using namespace std;

BaseService::BaseService(shared_ptr<KernelInterface> kernel)
 : kernel_(kernel) {
    auto data_store = make_shared<service::Datastore>(kernel->GetDatabaseManager()->getConnection("galaxy_manager"));
    service_directory_ = make_shared<service::ServiceDirectory>(data_store, kernel->GetEventDispatcher());

    auto swganh_kernel = static_pointer_cast<SwganhKernel>(kernel);
    service_directory_->joinGalaxy(swganh_kernel->GetAppConfig().galaxy_name, kernel_->GetVersion().ToString());
}

void BaseService::Start() {
    running_ = true;

    auto service_description = GetServiceDescription();
    if (!service_directory_->registerService(service_description.name(), service_description.type(), service_description.version(), service_description.address(), service_description.tcp_port(), service_description.udp_port(), service_description.ping_port())) {
        running_ = false;
        throw std::runtime_error("Unable to register service " + service_description.name());
    }

    subscribe();
    
    onStart();
}

void BaseService::Update() {
    service_directory_->pulse();

    onUpdate();
}

void BaseService::Stop() {
    running_ = false;
    onStop();
}

bool BaseService::IsRunning() const { return running_; }

shared_ptr<KernelInterface> BaseService::kernel() {
    return kernel_;
}

shared_ptr<service::ServiceDirectory> BaseService::service_directory() {
    return service_directory_;
}

shared_ptr<EventDispatcherInterface> BaseService::event_dispatcher() {
    return kernel_->GetEventDispatcher();
}
