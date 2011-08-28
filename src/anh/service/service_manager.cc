
#include "anh/service/service_manager.h"

#include <algorithm>
#include <string>
#include <stdexcept>

#include <boost/thread.hpp>

#include "anh/plugin/plugin_manager.h"
#include "anh/service/service_interface.h"

#ifndef WIN32
#include <boost/regex.hpp>
    using boost::regex;
    using boost::regex_search;
#else
    #include <regex>
    using std::regex;
    using std::regex_search;
#endif

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

void ServiceManager::Initialize() {
    auto registration_map = plugin_manager_->registration_map();

    regex rx("Service");

    for_each(registration_map.begin(), registration_map.end(), [this, &rx] (RegistrationMap::value_type& entry) {
        std::string name = entry.first;

        if (entry.first.length() > 7 && regex_search(name.begin(), name.end(), rx)) {
            auto service = GetService(entry.first);

            if (service) {
                services_.insert(make_pair(entry.first, service));
            }
        }
    });
}

void ServiceManager::Start() {
    for_each(services_.begin(), services_.end(), [] (ServiceMap::value_type& entry) {
       if (entry.second) {
           entry.second->Start();
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
