#ifndef ANH_MODULE_MANAGER_H
#define ANH_MODULE_MANAGER_H
#include <anh/hash_string.h>
#include <map>
#include <vector>

namespace anh {
namespace module_manager {
struct Module
{
public:
    void * module_instance;
    std::string module_name;

};
class ModuleLoader
{
public:
    std::shared_ptr<Module> LoadModule(anh::HashString name) {return std::make_shared<Module>();}
    void FreeModule(anh::HashString name) {}
};
class ModuleManager
{
public:
    ModuleManager(ModuleLoader module_loader){ loader_ = module_loader; }
    bool LoadModule(anh::HashString module_name);
    void LoadModules(std::vector<anh::HashString> modules);
    void UnloadModule(anh::HashString module_name);
    bool isLoaded(anh::HashString module_name);
private:
    typedef std::map<anh::HashString, std::shared_ptr<Module>>	                    ModuleLoaders;
    typedef std::map<anh::HashString, std::shared_ptr<Module>>::iterator	        ModuleIterator;
    typedef std::pair<anh::HashString, std::shared_ptr<Module>>                     ModulePair;
    ModuleLoaders loaded_modules_;
    ModuleLoader loader_;
};
} // module_manager
} // anh
#endif //ANH_MODULE_MANAGER_H