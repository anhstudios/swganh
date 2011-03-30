#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>
#include <string>

#include <anh\module_manager\module_interface.h>
#include <anh\module_manager\platform_services.h>

#define DLL_EXPORT __declspec(dllexport)

#ifdef __cplusplus
extern "C"
{
#endif

bool DLL_EXPORT Load(std::shared_ptr<anh::module_manager::PlatformServices>);
bool DLL_EXPORT Unload(std::shared_ptr<anh::module_manager::PlatformServices>);
const std::string DLL_EXPORT GetModuleName(void);
const anh::module_manager::ModuleApiVersion DLL_EXPORT GetModuleVersion(void);
const std::string DLL_EXPORT GetModuleDescription(void);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__