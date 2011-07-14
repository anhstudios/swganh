
#ifndef SWGANH_APP_SWGANH_APP_H_
#define SWGANH_APP_SWGANH_APP_H_

#include <memory>

#include "anh/app/app_interface.h"

namespace anh {
namespace app {
class KernelInterface;
}}  // namespace anh::app

namespace swganh {
namespace app {

class SwganhKernel;

class SwganhApp : public anh::app::AppInterface {
public:    
    SwganhApp();

    void Initialize(int argc, char* argv[]);

    void Start();

    void Stop();

    bool IsRunning();

    std::shared_ptr<anh::app::KernelInterface> GetAppKernel();

public:
    std::shared_ptr<SwganhKernel> kernel_;
    bool running_;
};

}}  // namespace swganh::app

#endif  // SWGANH_APP_SWGANH_APP_H_
