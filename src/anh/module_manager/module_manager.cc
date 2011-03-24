#include "module_manager.h"
#include <algorithm>
using namespace std;
namespace anh {
namespace module_manager {

bool ModuleManager::LoadModule(HashString module_name, void * params)
{
    // check if we have already loaded this module
    ModuleIterator i = loaded_modules_.find(module_name);
    if (i == loaded_modules_.end())
    {
        shared_ptr<Module> module;
        // load the library into an instance and pass into map for future use
        module = loader_->Load(module_name.ident_string(), params);
        if (module != nullptr)
        {
            loaded_modules_.insert(ModulePair(module_name, module));
            return true;
        }
    }
    return false;
}
void ModuleManager::LoadModules(ModulesMap modules)
{  
    for (auto it = modules.begin(); it != modules.end(); ++it)
    {
        LoadModule((*it).first, (*it).second);
    }
}
bool ModuleManager::isLoaded(HashString module_name)
{
    ModuleIterator i = loaded_modules_.find(module_name);
    if (i != loaded_modules_.end())
    {
        return true;
    }
    return false;
}
void ModuleManager::UnloadModule(HashString module_name)
{
    // check if this module is even loaded
    ModuleIterator i = loaded_modules_.find(module_name);
    if (i != loaded_modules_.end())
    {
        loader_->Unload(i->second);
        loaded_modules_.erase(i);
    }
}

} // module_manager
} // anh