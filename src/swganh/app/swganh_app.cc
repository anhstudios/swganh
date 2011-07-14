
#include "swganh/app/swganh_app.h"

#include "swganh/app/swganh_kernel.h"

using namespace anh::app;
using namespace swganh::app;

using std::make_shared;

SwganhApp::SwganhApp() {
    kernel_ = make_shared<SwganhKernel>();
}

void SwganhApp::Initialize(int argc, char* argv[]) {

}

void SwganhApp::Start() {

}

void SwganhApp::Stop() {

}

bool SwganhApp::IsRunning() {
    return running_;
}

std::shared_ptr<KernelInterface> SwganhApp::GetAppKernel() {
    return kernel_;
}
