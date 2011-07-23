
#ifndef SWGANH_APP_SWGANH_APP_H_
#define SWGANH_APP_SWGANH_APP_H_

#include <memory>
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <tbb/atomic.h>

#include "anh/app/app_interface.h"
#include "anh/service/service_directory.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

namespace swganh {
namespace app {

class SwganhKernel;

typedef std::pair<boost::program_options::options_description, boost::program_options::variables_map> ServiceConfig;

class SwganhApp : public anh::app::AppInterface, private boost::noncopyable {
public:    
    SwganhApp();

    void Initialize(int argc, char* argv[]);

    void Start();

    void Stop();

    bool IsRunning();

    std::shared_ptr<anh::app::KernelInterface> GetAppKernel();

private:
    void LoadAppConfig_(int argc, char* argv[]);
    void LoadServiceConfig_(ServiceConfig& service_config);
    void LoadPlugins_(std::vector<std::string> plugins);
    
    std::shared_ptr<SwganhKernel> kernel_;
    std::shared_ptr<anh::service::ServiceDirectory> service_directory_;
    tbb::atomic<bool> running_;
    bool initialized_;
    
    ServiceConfig service_config_;
};

}}  // namespace swganh::app

#endif  // SWGANH_APP_SWGANH_APP_H_
