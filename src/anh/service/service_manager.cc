
#include "anh/service/service_manager.h"

#include <algorithm>
#include <regex>
#include <string>
#include <stdexcept>

#include <boost/thread.hpp>

#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_interface.h"

//using namespace anh::plugin;
using namespace anh::service;
using namespace anh::plugin;
using namespace std;

ServiceManager::ServiceManager(shared_ptr<PluginManager> plugin_manager)
    : plugin_manager_(plugin_manager) {}

shared_ptr<ServiceInterface> ServiceManager::GetService(string name) {
    auto it = services_.find(name);

    if (it != services_.end()) {
        return it->second;
    }

    auto service = plugin_manager_->CreateObject<ServiceInterface>(name);

    if (!service) {
        // throw exception here, service already exists
        throw std::runtime_error("Unknown service requested: " + name);
        return nullptr;
    }

    return service;
}

void ServiceManager::AddService(string name, shared_ptr<ServiceInterface> service) {
    auto current_service = GetService(name);

    if (current_service) {
        // throw exception here, service already exists
        throw std::runtime_error("service already exists: " + name);
        return;
    }

    services_[name] = service;
}

void ServiceManager::Start() {
    auto registration_map = plugin_manager_->registration_map();

    std::regex rx("Service");
    
    for_each(registration_map.begin(), registration_map.end(), [this, &rx] (RegistrationMap::value_type& entry) {
        std::string name = entry.first;

        if (entry.first.length() > 7 && std::regex_search(name.begin(), name.end(), rx)) {
            auto service = GetService(entry.first);
            services_.insert(make_pair(entry.first, service));
        }
    });

    for_each(services_.begin(), services_.end(), [] (ServiceMap::value_type& entry) {
       if (entry.second) {
           boost::thread t([&entry] () { entry.second->Start(); });
       } 
    });    
}

void ServiceManager::Stop() {
    for_each(services_.begin(), services_.end(), [] (ServiceMap::value_type& entry) {
       if (entry.second) {
           entry.second->Stop();
       } 
    });
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