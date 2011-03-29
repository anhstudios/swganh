#include "module_manager.h"
#if _WIN32
#include "win32_module.h"
#endif
#if __unix
#include "unix_module.h"
#endif
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;
namespace fs = boost::filesystem;
namespace anh {
namespace module_manager {

shared_ptr<ModuleInterface> ModuleManager::CreateModule()
{
    shared_ptr<ModuleInterface> module;
    #if _WIN32
        module = make_shared<Win32Module>();
    #endif
    #if __unix
        //module = make_shared<UnixModule>();
    #endif

    return module;
}

bool ModuleManager::LoadModule(HashString module_name, shared_ptr<PlatformServices> platform_services)
{
    // check if we have already loaded this module
    ModuleIterator i = loaded_modules_.find(module_name);
    if (i == loaded_modules_.end())
    {
        shared_ptr<ModuleInterface> module = CreateModule();
        // load the library into an instance and pass into map for future use
        module->Load(module_name.ident_string(), platform_services);
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
void ModuleManager::LoadModules(const string& file_name)
{
    ModulesMap modules;
    modules = CreateModulesMapFromFile_(file_name);
    if (modules.size() > 0)
    {
        LoadModules(modules);
    }
}
ModulesMap ModuleManager::CreateModulesMapFromFile_(const string& file_name)
{
    fs::path file (file_name);
    ModulesMap modules_map;
    if(fs::exists(file))
    {
        if (fs::is_regular_file(file))
        {
            fs::ifstream in_stream(file);
            vector<string> hs;
            // this is copying from the file stream each string line into the vector hs
            copy(istream_iterator<string>(in_stream), istream_iterator<string>(), back_inserter(hs));
            if (hs.size() > 0)
            {
                // loops through each vector strign and inserts into modules_map
                // TODO: change nullptr to something meaningful
                for_each(hs.begin(), hs.end(), [&modules_map] (string mod_name){
                    modules_map.insert(ModulesPair(HashString(mod_name), nullptr));
                });
            }
        }
    }
    else
    {
        throw runtime_error("Could not load file " + file_name);
    }
    return modules_map;
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
        (*i).second->Unload(services_);
        loaded_modules_.erase(i);
    }
}

} // module_manager
} // anh