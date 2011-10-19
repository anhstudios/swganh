
#include "anh/service/service_manager.h"

#include <algorithm>
#include <string>
#include <stdexcept>

#include <boost/thread.hpp>

#include "anh/service/service_directory_interface.h"
#include "anh/service/service_description.h"

#ifndef WIN32
#include <boost/regex.hpp>
    using boost::regex;
    using boost::regex_search;
#else
    #include <regex>
    using std::regex;
    using std::regex_search;
#endif

using namespace anh::service;
using namespace std;

ServiceManager::ServiceManager(const shared_ptr<ServiceDirectoryInterface>& service_directory) 
    : service_directory_(service_directory)
{}

shared_ptr<ServiceInterface> ServiceManager::GetService(string name) {
    auto it = services_.find(name);

    if (it == services_.end()) {
        return nullptr;
    }

    return it->second;
}

void ServiceManager::AddService(string name, shared_ptr<ServiceInterface> service) {
    auto current_service = GetService(name);

    if (current_service) {
        // throw exception here, service already exists
        throw std::runtime_error("service already exists: " + name);
        return;
    }

    auto service_description = service->GetServiceDescription();
    if (!service_directory_->registerService(service_description.name(), service_description.type(), service_description.version(), service_description.address(), service_description.tcp_port(), service_description.udp_port(), service_description.ping_port())) {
        throw std::runtime_error("Unable to register service " + service_description.name());
    }
            
    // update the status of the service
    service_description.status(anh::service::Galaxy::ONLINE);
    service_directory_->updateService(service_description);

    services_[name] = service;
}

void ServiceManager::Start() {
    for_each(services_.begin(), services_.end(), [] (ServiceMap::value_type& entry) {
       if (entry.second) {
           entry.second->Start();
       } 
    });    
}

void ServiceManager::Stop() {
    for_each(services_.begin(), services_.end(), [this] (ServiceMap::value_type& entry) {
        if (entry.second) {
            entry.second->Stop();
        } 
    });
}

const shared_ptr<ServiceDirectoryInterface>& ServiceManager::GetServiceDirectory() const
{
    return service_directory_;
}
