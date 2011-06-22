#include "main.h"

#include <iostream>
#include <tbb/compat/thread>

using namespace std;

bool API Load(std::shared_ptr<anh::module_manager::PlatformServices> services) {
    cout << GetModuleName() << " Loading..." <<endl;
    
    return true;
}

bool API Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {
    return true;
}

const std::string API GetModuleName(void) {
    return "Login Service";
}

const anh::module_manager::ModuleApiVersion API GetModuleVersion(void) {
    return anh::module_manager::ModuleApiVersion(0, 1, "0.1");
}

const std::string API GetModuleDescription(void) {
    return "Provides client login services to the cluster.";
}
