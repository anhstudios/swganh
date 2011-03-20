#include "module_manager.h"
using namespace std;
namespace anh {
namespace module_manager {

bool ModuleManager::LoadModule(const string& module)
{
    // check if we have already loaded this module
    ModuleIterator i = loaded_modules_.find(module);
    if (i != loaded_modules_.end())
    {
        shared_ptr<Package> package;
        // load the library into an instance and pass into map for future use
        // package = module_loader.loadModule(module);
        //instance = make_shared<Package>(LoadLibrary(module.c_str()));
        if (package != nullptr)
        {
            loaded_modules_.insert(ModulePair(module, package));
            return true;
        }
    }
    return false;
}
void ModuleManager::LoadModules(const vector<string> modules)
{
    vector<string>::const_iterator pos;
    for(pos = modules.begin(); pos != modules.end(); ++pos)
    {
        LoadModule(*pos);
    }
}
bool ModuleManager::UnloadModule(const string& module)
{
    // check if this module is even loaded
    ModuleIterator i = loaded_modules_.find(module);
    if (i != loaded_modules_.end())
    {
        // module_loader.FreeModule(*i->second.get());
        loaded_modules_.erase(i);
        return true;
    }
    return false;
}

} // module_manager
} // anh