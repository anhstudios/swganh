#ifndef ANH_MODULE_MANAGER_H
#define ANH_MODULE_MANAGER_H

#include <map>
#include <vector>
#ifdef _WIN32 // win specific include
#include <windows.h>
#endif
namespace anh {
namespace module_manager {
class Package;
class ModuleManager
{
public:
    bool LoadModule(const std::string& module_name);
    void LoadModules(const std::vector<std::string> modules);
    bool UnloadModule(const std::string& module_name);
private:
    typedef std::map<const std::string, std::shared_ptr<Package>>	        ModuleLoaders;
    typedef std::pair<const std::string, std::shared_ptr<Package>>          ModulePair;
    typedef std::map<const std::string, std::shared_ptr<Package>>::iterator	ModuleIterator;
    ModuleLoaders loaded_modules_;
    
};
} // module_manager
} // anh
#endif //ANH_MODULE_MANAGER_H