
#ifndef ANH_APP_KERNEL_INTERFACE_H_
#define ANH_APP_KERNEL_INTERFACE_H_

#include <cstdint>
#include <memory>
#include <string>

#include <boost/asio/io_service.hpp>

namespace anh {
namespace database {
    class DatabaseManagerInterface;
}}  // namespace anh::database

namespace anh {
    class EventDispatcher;
}  // namespace anh

namespace anh {
namespace plugin {
    class PluginManager;
}}  // namespace anh::plugin

namespace anh {
namespace service {
    class ServiceDirectoryInterface;
    class ServiceManager;
}}  // namespace anh::service

namespace anh {
namespace app {

struct Version {
    int32_t major;
    int32_t minor;

    std::string ToString() const {
        return  std::to_string(major) + "." + std::to_string(minor);
    }
};

class KernelInterface {
public:
    virtual ~KernelInterface() {}

    virtual const Version& GetVersion() = 0;
    
    virtual anh::EventDispatcher* GetEventDispatcher() = 0;
    
    virtual anh::plugin::PluginManager* GetPluginManager() = 0;

    virtual anh::service::ServiceManager* GetServiceManager() = 0;

    virtual anh::service::ServiceDirectoryInterface* GetServiceDirectory() = 0;

    virtual anh::database::DatabaseManagerInterface* GetDatabaseManager() = 0;
    
    virtual boost::asio::io_service& GetIoService() = 0;

    // also add entity manager, blah blah.
};

}}  // namespace anh::app

#endif  // ANH_APP_KERNEL_INTERFACE_H_