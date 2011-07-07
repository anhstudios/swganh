#include "main.h"

bool API Load(std::shared_ptr<anh::module_manager::PlatformServices> services) {
	return true;
}

void API Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {
}

void API Start(std::shared_ptr<anh::module_manager::PlatformServices> services) {    
}

void API Stop(std::shared_ptr<anh::module_manager::PlatformServices> services) {  
}

const std::string API GetModuleName(void) {
    return "DLLTest";
}

const std::string API GetModuleType(void) {
    return "DLLTest";
}

const anh::module_manager::ModuleApiVersion API GetModuleVersion(void) {
	return anh::module_manager::ModuleApiVersion(0, 1, "0.1");
}

const std::string API GetModuleDescription(void) {
	return "A test module.";
}
