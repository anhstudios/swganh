
#include "anh/service/service_manager.h"

#include <algorithm>
#include <stdexcept>

#include "anh/service/service_interface.h"

//using namespace anh::plugin;
using namespace anh::service;
using namespace std;

shared_ptr<ServiceInterface> ServiceManager::GetService(string name) {
    auto it = services_.find(name);

    if (it != services_.end()) {
        return it->second;
    }

    return nullptr;
}
    
void ServiceManager::AddService(string name, shared_ptr<ServiceInterface> service) {
    auto current_service = GetService(name);

    if (current_service) {
        // throw exception here, service already exists
        throw std::runtime_error("service " + name + " already exists");
        return;
    }

    services_[name] = service;
}


//
//void ServiceManager::RegisterModuleServices(std::shared_ptr<ModuleManager> module_manager) {
//    auto registration_map = module_manager->registration_map();
//    
//    for_each(registration_map.begin(), registration_map.end(), [module_manager] (RegistrationMap::value_type& entry) {
//        if (entry->first.substr(-7, 7) == "Service") {
//            auto service = module_manager->CreateObject<ServiceInterface>(entry->first);
//            services_.push_back(service);
//        }
//    });
//}
//
//void ServiceManager::RegisterService(std::shared_ptr<ServiceInterface> service) {
//    services_.push_back(service);
//}
//
//void ServiceManager::StartServices() {
//    for_each(services_.begin(), services_.end(), [] (shared_ptr<ServiceInterface> service) {
//       if (service) {
//           service->Start();
//       } 
//    });
//}
//
//void ServiceManager::StopServices() {    
//    for_each(services_.begin(), services_.end(), [] (shared_ptr<ServiceInterface> service) {
//       if (service) {
//           service->Stop();
//       } 
//    });
//}