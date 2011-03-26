#include "module_manager.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;
namespace fs = boost::filesystem;
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
        loader_->Unload(i->second);
        loaded_modules_.erase(i);
    }
}

} // module_manager
} // anh