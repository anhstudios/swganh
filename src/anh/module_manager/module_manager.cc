/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/
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
bool ModuleManager::LoadModule(std::string file_name, std::shared_ptr<ModuleInterface> module)
{
    // load the library into an instance and pass into map for future use
    if (module->Load(file_name, services_, version_))
    {
        loaded_modules_.insert(ModulePair(file_name, module));
        return true;
    }
    return false;
}
bool ModuleManager::LoadModule(HashString module_name)
{
    shared_ptr<ModuleInterface> module = CreateModule();
    // load the library into an instance and pass into map for future use
    if (LoadModule(module_name.ident_string(), module))
    {
        return true;
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
void ModuleManager::LoadModules(ModulesVec modules_vec)
{
    for_each(modules_vec.begin(), modules_vec.end(), [&] (string file_name) {
        // get new module of appropriate type
        auto module = CreateModule();
        // load module
        if (module->Load(file_name, services_, version_)) 
        {
            // add module to our stored modules map
            loaded_modules_.insert(ModulesPair(file_name, module));
        }
        // log couldn't load module
    });
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
                // loops through each vector string and inserts into modules_vector
                for_each(hs.begin(), hs.end(), [=,&modules_map] (string mod_name){
                    auto module = CreateModule();
                    module->Load(mod_name, services_, version_);
                    loaded_modules_.insert(ModulesPair(mod_name, module));
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