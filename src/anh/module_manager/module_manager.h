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

#ifndef ANH_MODULE_MANAGER_MODULE_MANAGER_H
#define ANH_MODULE_MANAGER_MODULE_MANAGER_H
#include <anh/hash_string.h>
#include <anh/module_manager/module_interface.h>
#include <map>
#include <vector>

namespace anh {
namespace module_manager {
class PlatformServices;
typedef std::map<anh::HashString, std::shared_ptr<ModuleInterface>>	               ModuleLoaders;
typedef std::map<anh::HashString, std::shared_ptr<ModuleInterface>>::iterator	   ModuleIterator;
typedef std::pair<anh::HashString, std::shared_ptr<ModuleInterface>>               ModulePair;
typedef std::vector<std::string>                                                   ModulesVec;

/// \brief The Module Manager handles access to modules
///
class ModuleManager
{
public:
    /// \brief default ctor
    ModuleManager(std::shared_ptr<PlatformServices> services, std::shared_ptr<ModuleApiVersion> api_version) {
        services_ = services; 
        version_ = api_version;
    }
    ~ModuleManager();
    /*  \brief creates a module depending on the OS and returns it
    *
    *   \returns shared_ptr module either Win32Module, UnixModule, or MacOSModule
    */
    std::shared_ptr<ModuleInterface> CreateModule();
    /*  \brief Load a module from a module object
    *
    *   \param[in] module object to load into memory
    */
    bool LoadModule(std::string name, std::shared_ptr<ModuleInterface> module);
    /*  \brief Loads a module, passes module_name and stores values in ModuleLoaders
    *
    *   \param[in] HashString of the module name to be loaded
    *   \returns true if the load was successful
    */
    bool LoadModule(anh::HashString module_name);
    /// loads a vector of modules
    void LoadModules(ModulesVec modules_vec);
    /* \brief Loads modules from a plaintext file
    *  \param std::string of text file to be loaded
    *  \calls @LoadModule
    */
    void LoadModules(const std::string& file_name);

    void StartModule(anh::HashString module_name);
    void StartModules();

    void StopModule(anh::HashString module_name);
    void StopModules();

    /* \brief Unloads module based on HashString name
    *  \param[in] HashString module name to unload
    */
    void UnloadModule(anh::HashString module_name);
    // unloads all modules in map
    void UnloadModules();
    /* \brief Checks to see if given module_name is loaded
    *  \param[in] HashString module_name of module to determine if loaded
    *  \returns true if module is already loaded.
    */
    bool isLoaded(anh::HashString module_name);

private:
    /// \brief the map of the currently loaded modules
    ModuleLoaders loaded_modules_;
    /// \brief shared_ptr of the shared services
    std::shared_ptr<PlatformServices> services_;
    std::shared_ptr<ModuleApiVersion> version_;
};
} // module_manager
} // anh
#endif //ANH_MODULE_MANAGER_MODULE_MANAGER_H