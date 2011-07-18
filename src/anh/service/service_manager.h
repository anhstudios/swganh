
#ifndef ANH_SERVICE_SERVICE_MANAGER_H_
#define ANH_SERVICE_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>

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

//class ServiceInterface;
//    
//class ServiceManager {
//public:    
//    void RegisterPluginServices(std::shared_ptr<anh::plugin::PluginManager> module_manager);
//    void RegisterService(std::shared_ptr<ServiceInterface> service);
//    
//    void StartServices();
//    void StopServices();
//    
//private:
//    typedef std::vector<std::shared_ptr<ServiceInterface>> ServiceContainer;
//    ServiceContainer services_;
//};
    
}}  // namespace anh::service

#endif  // ANH_SERVICE_SERVICE_MANAGER_H_
