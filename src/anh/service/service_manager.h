
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

    void AddService(std::string name, std::shared_ptr<ServiceInterface> service);

    std::shared_ptr<ServiceInterface> GetService(std::string name);

    template<typename T>
    std::shared_ptr<T> GetService(std::string name)
    {
        std::shared_ptr<T> service;

        auto tmp = GetService(name);

#ifdef _DEBUG
        service = std::dynamic_pointer_cast<T>(tmp);
#else
        service = std::static_pointer_cast<T>(tmp);
#endif

        return service;
    }

    template<typename T>
    std::vector<std::shared_ptr<T>> GetServicesByType(std::string type_name)
    {
        std::vector<std::shared_ptr<T>> services;
        std::shared_ptr<T> tmp;

        std::for_each(services_.begin(),
            services_.end(),
            [&services, &type_name, &tmp] (ServiceMap::value_type& entry)
        {
            anh::service::ServiceDescription description = entry.second.second->GetServiceDescription();

            if (description.type().compare(type_name) == 0) 

            {
#ifdef _DEBUG
                tmp = std::dynamic_pointer_cast<T>(entry.second);
#else
                tmp = std::static_pointer_cast<T>(entry.second);
#endif
                services.push_back(tmp);
            }
        });

        return services;
    }

    // add start/stop services, all and individually
    void Start();
    void Stop();

private:
    typedef std::map<std::string, std::pair<
        std::shared_ptr<ServiceDescription>, 
        std::shared_ptr<ServiceInterface>>
    > ServiceMap;
    ServiceMap services_;

    ServiceDirectoryInterface* service_directory_;
};
    
}}  // namespace anh::service

#endif  // ANH_SERVICE_SERVICE_MANAGER_H_
