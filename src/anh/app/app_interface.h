
#ifndef ANH_APP_APP_INTERFACE_H_
#define ANH_APP_APP_INTERFACE_H_

namespace anh {
namespace app {

class KernelInterface;

class AppInterface {
public:
    virtual ~AppInterface() {}

    virtual void Initialize(int argc, char* argv[]) = 0;

    virtual void Start() = 0;

    virtual void Stop() = 0;

    virtual bool IsRunning() = 0;

    virtual std::shared_ptr<KernelInterface> GetAppKernel() = 0;
};

}}  // namespace anh::app

#endif  // ANH_APP_APP_INTEFACE_H_
