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
#ifdef _WIN32
#include "win32_module.h"
#endif
#ifdef UNIX
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

ModuleManager::~ModuleManager()
{
    UnloadModules();
}
shared_ptr<ModuleInterface> ModuleManager::CreateModule()
{
    shared_ptr<ModuleInterface> module;
    #if _WIN32
        module = make_shared<Win32Module>();
    #endif
    #if _unix
        module = make_shared<UnixModule>();
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
    auto module = CreateModule();
    // load the library into an instance and pass into map for future use
    if (LoadModule(module_name.ident_string(), module))
    {
        return true;
    }
    return false;
}
void ModuleManager::LoadModules(const string& file_name)
{
    fs::path file (file_name);
    ModuleLoaders modules_map;
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
                LoadModules(hs);
            }
        }
    }
    else
    {
        throw runtime_error("Could not load file " + file_name);
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
            loaded_modules_.insert(ModulePair(HashString(file_name), module));
        }
        // log couldn't load module
    });
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


void ModuleManager::StartModule(anh::HashString module_name) {
    // check if this module is even loaded
    ModuleIterator i = loaded_modules_.find(module_name);
    if (i != loaded_modules_.end())
    {
        (*i).second->Start(services_);
    }
}

void ModuleManager::StartModules() {
    for_each(loaded_modules_.begin(), loaded_modules_.end(), [&] (ModulePair module) {
        module.second->Start(services_);
    });
}

void ModuleManager::StopModule(anh::HashString module_name) {
    // check if this module is even loaded
    ModuleIterator i = loaded_modules_.find(module_name);
    if (i != loaded_modules_.end())
    {
        (*i).second->Stop(services_);
    }
}

void ModuleManager::StopModules() {
    for_each(loaded_modules_.begin(), loaded_modules_.end(), [&] (ModulePair module) {
        module.second->Stop(services_);
    });
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
void ModuleManager::UnloadModules()
{
    for_each(loaded_modules_.begin(), loaded_modules_.end(), [&] (ModulePair module) {
        module.second->Unload(services_);
    });
    loaded_modules_.empty();
}

} // module_manager
} // anh
