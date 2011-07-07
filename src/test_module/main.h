#ifndef __MAIN_H__
#define __MAIN_H__

#include <string>

#include <anh/module_manager/module_interface.h>
#include <anh/module_manager/platform_services.h>
#include <anh/module_manager/module_main.h>

#ifdef __cplusplus
extern "C"
{
#endif

bool API Load(std::shared_ptr<anh::module_manager::PlatformServices>);
void API Unload(std::shared_ptr<anh::module_manager::PlatformServices>);
void API Start(std::shared_ptr<anh::module_manager::PlatformServices>);
void API Stop(std::shared_ptr<anh::module_manager::PlatformServices>);
const std::string API GetModuleName(void);
const std::string API GetModuleType(void);
const anh::module_manager::ModuleApiVersion API GetModuleVersion(void);
const std::string API GetModuleDescription(void);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
