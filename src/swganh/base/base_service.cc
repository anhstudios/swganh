// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "swganh/base/base_service.h"

#include "anh/app/kernel_interface.h"
#include "anh/database/database_manager_interface.h"

#include "anh/service/datastore.h"
#include "anh/service/galaxy.h"

#include "swganh/app/swganh_kernel.h"

using namespace anh;
using namespace app;
using namespace swganh::app;
using namespace swganh::base;
using namespace event_dispatcher;
using namespace std;

BaseService::BaseService(swganh::app::SwganhKernel* kernel)
    : kernel_(kernel)
    , active_(kernel->GetIoService()) 
{}

void BaseService::Start() {
        running_ = true;
        
        subscribe();
        
        onStart();
}

void BaseService::Stop() {
        running_ = false;
        
        onStop();
}

bool BaseService::IsRunning() const { return running_; }

SwganhKernel* BaseService::kernel() {
    return kernel_;
}

ActiveObject& BaseService::active() {
    return active_;
}