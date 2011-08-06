
#include "anh/plugin/plugin_manager.h"

#include <algorithm>
#include <iostream>

#include <boost/filesystem.hpp>

#include "anh/app/kernel_interface.h"

#include "anh/plugin/platform/library_interface.h"

#ifdef WIN32
#include "anh/plugin/platform/win32_library.h"
typedef anh::plugin::platform::Win32Library DynamicLibrary;
#else
#include "anh/plugin/platform/posix_library.h"
typedef anh::plugin::platform::PosixLibrary DynamicLibrary;
#endif

#if defined(__APPLE__)
  static std::string library_extension(".dylib");
#elif defined(WIN32)
  static std::string library_extension(".dll");
#else
  static std::string library_extension(".so");
#endif

using namespace anh::app;
using namespace anh::plugin;
using namespace std;

PluginManager::PluginManager(shared_ptr<KernelInterface> kernel)
    : kernel_(kernel) 
{
}

PluginManager::~PluginManager() {    
    auto end = exit_funcs_.end();
    std::for_each(exit_funcs_.begin(), exit_funcs_.end(), [] (ExitFunc exit_func) {
        try {
            (*exit_func)();
        } catch(...) {
            // Report error here.
        }
    });
}

bool PluginManager::LoadPlugin(const std::string& path) {
    if (library_map_.find(path) != library_map_.end()) {
        cout << "Step 1 failed " << path << "\n" << endl;
        return false;
    }

    auto library = LoadLibrary_(path);
    if (!library) {
        cout << "Step 3 failed " << path << "\n" << endl;
        return false;
    }

    InitFunc init_func = library->GetSymbol<InitFunc>("InitializePlugin");
    if (!init_func) {
        cout << "Step 4 failed " << path << "\n" << endl;
        return false;
    }

    if (!InitializePlugin(init_func)) {
        cout << "Step 5 failed " << path << "\n" << endl;
        return false;
    }

    library_map_[path] = library;

    return true;
}

bool PluginManager::LoadAllPlugins(const std::string& directory) {
    boost::filesystem::path plugin_dir(directory);

    bool all_loaded = true;

    try {
        if (!boost::filesystem::exists(plugin_dir) ||
            !boost::filesystem::is_directory(plugin_dir)) {
            all_loaded = false;
        }

        std::for_each(boost::filesystem::directory_iterator(plugin_dir),
            boost::filesystem::directory_iterator(),
            [this, &all_loaded] (const boost::filesystem::directory_entry& entry) 
        {            
            if (entry.path().extension() != library_extension) {
                return;
            }

            if (!this->LoadPlugin(entry.path().native())) {
                 all_loaded = false;
            }
        });

    } catch(const boost::filesystem::filesystem_error& /* e */) {
        // do some reporting;
        return false;
    }

    return all_loaded;
}

bool PluginManager::InitializePlugin(InitFunc init_func) {
    if (!init_func) {
        return false;
    }

    ExitFunc exit_func = init_func(kernel_);

    if (!exit_func) {
        return false;
    }

    exit_funcs_.push_back(exit_func);

    return true;
}

bool PluginManager::RegisterObject(const std::string& name, const ObjectRegistration* registration) {
    if (name.empty()) {
        return false;
    }

    if (!registration || !registration->CreateObject || !registration->DestroyObject) {
        return false;
    }

    anh::app::Version version = kernel_->GetVersion();
    if (version.major != registration->version.major) {
        return false;
    }

    if (registration_map_.find(name) != registration_map_.end()) {
        return false;
    }

    registration_map_[name] = *registration;

    return true;
}

const RegistrationMap& PluginManager::registration_map() {
    return registration_map_;
}

std::shared_ptr<platform::LibraryInterface> PluginManager::LoadLibrary_(const std::string& path) {
    auto library = DynamicLibrary::Load(path);

    if (!library) {
        return nullptr;
    }

    return library;
}
