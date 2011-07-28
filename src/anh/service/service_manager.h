
#ifndef ANH_SERVICE_SERVICE_MANAGER_H_
#define ANH_SERVICE_SERVICE_MANAGER_H_

#include <map>
#include <memory>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace anh {
namespace plugin {
class PluginManager;
}}  // namespace anh::plugin

namespace anh {
namespace service {

class ServiceInterface;
    
typedef std::pair<boost::program_options::options_description, boost::program_options::variables_map> ServiceConfig;

class ServiceManager {
public:
    explicit ServiceManager(std::shared_ptr<anh::plugin::PluginManager> plugin_manager);

    void Initialize(ServiceConfig& service_config);

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
