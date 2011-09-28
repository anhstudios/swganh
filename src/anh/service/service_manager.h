
#ifndef ANH_SERVICE_SERVICE_MANAGER_H_
#define ANH_SERVICE_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace anh {
namespace plugin {
class PluginManager;
}}  // namespace anh::plugin

namespace anh {
namespace service {

class ServiceInterface;
    
class ServiceManager {
public:
    explicit ServiceManager(std::shared_ptr<anh::plugin::PluginManager> plugin_manager);

    void Initialize();
    
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
            anh::service::ServiceDescription description = entry.second->GetServiceDescription();

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

    std::shared_ptr<ServiceInterface> GetService(std::string name);
    void AddService(std::string name, std::shared_ptr<ServiceInterface> service);

    // add start/stop services, all and individually
    void Start();
    void Stop();

private:
    typedef std::map<std::string, std::shared_ptr<ServiceInterface>> ServiceMap;
    ServiceMap services_;

    std::shared_ptr<anh::plugin::PluginManager> plugin_manager_;
};
    
}}  // namespace anh::service

#endif  // ANH_SERVICE_SERVICE_MANAGER_H_
