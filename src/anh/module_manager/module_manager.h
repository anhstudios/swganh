#ifndef ANH_MODULE_MANAGER_MODULE_MANAGER_H
#define ANH_MODULE_MANAGER_MODULE_MANAGER_H
#include <anh/hash_string.h>
#include <anh/module_manager/module_interface.h>
#include <map>
#include <vector>

class PlatformServices;
namespace anh {
namespace module_manager {

typedef std::map<anh::HashString, std::shared_ptr<PlatformServices> >                       ModulesMap;
typedef std::map<anh::HashString, std::shared_ptr<PlatformServices> >::iterator             ModulesIterator;
typedef std::pair<anh::HashString, std::shared_ptr<PlatformServices> >                      ModulesPair;

/// \brief The Module Manager handles access to modules
///
class ModuleManager
{
public:
    /// \brief default ctor
    ModuleManager() {};
    /// \brief creates a module depending on the OS and returns it
    ///
    /// \returns shared_ptr module either Win32Module, UnixModule, or MacOSModule
    std::shared_ptr<ModuleInterface> CreateModule();
    /// \brief Loads a module, passes services and stores values in ModuleLoaders
    ///
    /// \param[in] HashString of the module name to be loaded
    /// \param[in] PlatformServices object containing services to pass to module
    /// \returns true if the load was successful
    bool LoadModule(anh::HashString module_name, std::shared_ptr<PlatformServices> platform_services);
    /// \brief Loads a map of modules
    /// \param[in] ModulesMap of modules to be loaded
    /// \calls @LoadModule
    void LoadModules(ModulesMap modules);
    /// \brief Loads modules from a plaintext file
    /// \param std::string of text file to be loaded
    /// \calls @LoadModule
    void LoadModules(const std::string& file_name);
    /// \brief Unloads module based on HashString name
    /// \param[in] HashString module name to unload
    void UnloadModule(anh::HashString module_name);
    /// \brief Checks to see if given module_name is loaded
    /// \param[in] HashString module_name of module to determine if loaded
    /// \returns true if module is already loaded.
    bool isLoaded(anh::HashString module_name);
private:
    typedef std::map<anh::HashString, std::shared_ptr<ModuleInterface>>	               ModuleLoaders;
    typedef std::map<anh::HashString, std::shared_ptr<ModuleInterface>>::iterator	   ModuleIterator;
    typedef std::pair<anh::HashString, std::shared_ptr<ModuleInterface>>                ModulePair;
    /// \brief Creates a ModulesMap from a plaintext file, used in @LoadModules(file_name)
    /// \param std::string file_name the file to create map from.
    /// \returns ModulesMap of modules.
    ModulesMap CreateModulesMapFromFile_(const std::string& file_name);
    /// \brief the map of the currently loaded modules
    ModuleLoaders loaded_modules_;
    /// \brief shared_ptr of the shared services
    std::shared_ptr<PlatformServices> services_;
};
} // module_manager
} // anh
#endif //ANH_MODULE_MANAGER_MODULE_MANAGER_H