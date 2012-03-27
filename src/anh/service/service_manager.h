
#ifndef ANH_SERVICE_SERVICE_MANAGER_H_
#define ANH_SERVICE_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "anh/service/service_interface.h"

namespace anh {
namespace database {
    class DatabaseManagerInterface;
}}  // namespace anh::database

namespace anh {
namespace service {

class ServiceDirectoryInterface;

class ServiceManager {
public:
    explicit ServiceManager(ServiceDirectoryInterface* service_directory);
    
    ~ServiceManager();

    void AddService(std::string name, std::unique_ptr<ServiceInterface> service);

    ServiceInterface* GetService(std::string name);

    template<typename T>
    T* GetService(std::string name)
    {
        T* service;

        auto tmp = GetService(name);

#ifdef _DEBUG
        service = dynamic_cast<T*>(tmp);
#else
        service = static_cast<T*>(tmp);
#endif

        return service;
    }

    template<typename T>
    std::vector<T*> GetServicesByType(std::string type_name)
    {
        std::vector<T*> services;
        T* tmp;

        for(auto& entry : services_)
        {
            anh::service::ServiceDescription description = entry.second.second->GetServiceDescription();

            if (description.type().compare(type_name) == 0) 

            {
#ifdef _DEBUG
                tmp = dynamic_cast<T>(entry.second.second.get());
#else
                tmp = static_cast<T>(entry.second.second.get());
#endif
                services.push_back(tmp);
            }
        }

        return services;
    }

    // add start/stop services, all and individually
    void Start();
    void Stop();

private:
    typedef std::map<std::string, std::pair<
        std::shared_ptr<ServiceDescription>, 
        std::unique_ptr<ServiceInterface>>
    > ServiceMap;
    ServiceMap services_;

    ServiceDirectoryInterface* service_directory_;
};
    
}}  // namespace anh::service

#endif  // ANH_SERVICE_SERVICE_MANAGER_H_
