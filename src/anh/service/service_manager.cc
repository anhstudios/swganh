
#include "anh/service/service_manager.h"

#include <algorithm>
#include <string>
#include <stdexcept>

#include <boost/thread.hpp>

#include "anh/service/service_directory_interface.h"

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
    for_each(services_.begin(), services_.end(), [] (ServiceMap::value_type& entry) {
       if (entry.second) {
           entry.second->Stop();
       } 
    });
}

const shared_ptr<ServiceDirectoryInterface>& ServiceManager::GetServiceDirectory() const
{
    return service_directory_;
}
