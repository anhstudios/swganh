#ifndef ANH_MODULE_MANAGER_MODULE_MANAGER_H
#define ANH_MODULE_MANAGER_MODULE_MANAGER_H
#include <anh/hash_string.h>
#include <anh/module_manager/module_loader_interface.h>
#include <map>
#include <vector>

namespace anh {
namespace module_manager {

typedef std::map<anh::HashString, void*>                                       ModulesMap;
typedef std::map<anh::HashString, void*>::iterator                             ModulesIterator;
typedef std::pair<anh::HashString, void*>                                      ModulesPair;

class ModuleManager
{

private:
    typedef std::map<anh::HashString, std::shared_ptr<Module>>	               ModuleLoaders;
    typedef std::map<anh::HashString, std::shared_ptr<Module>>::iterator	   ModuleIterator;
    typedef std::pair<anh::HashString, std::shared_ptr<Module>>                ModulePair;

    ModuleLoaders loaded_modules_;
    std::shared_ptr<ModuleLoaderInterface> loader_;

public:
    ModuleManager(std::shared_ptr<ModuleLoaderInterface> module_loader){ loader_ = module_loader; }
    bool LoadModule(anh::HashString module_name, void * params);
    void LoadModules(ModulesMap modules);
    void UnloadModule(anh::HashString module_name);
    bool isLoaded(anh::HashString module_name);
};
} // module_manager
} // anh
#endif //ANH_MODULE_MANAGER_MODULE_MANAGER_H