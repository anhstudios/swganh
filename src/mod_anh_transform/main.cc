#include "main.h"

bool DLL_EXPORT Load(std::shared_ptr<anh::module_manager::PlatformServices> services) {
	// register component to object manager

    // subscribe to events

    // trigger 'loaded event'

    // init specific logs

    return true;
}

bool DLL_EXPORT Unload(std::shared_ptr<anh::module_manager::PlatformServices> services) {
	// unregister components

    // unsubscribes

    // triggers 'unload event'
    return true;
}

const std::string DLL_EXPORT GetModuleName(void) {
	return "ANH.Transform";
}

const anh::module_manager::ModuleApiVersion DLL_EXPORT GetModuleVersion(void) {
	return anh::module_manager::ModuleApiVersion(0, 1, "0.1");
}

const std::string DLL_EXPORT GetModuleDescription(void) {
	return "ANH's Transform Module";
}