
#include "anh/module/module_manager.h"

#include <algorithm>
#include <iostream>

#include <boost/filesystem.hpp>

#include "anh/module/platform/library_interface.h"

#ifdef WIN32
#include "anh/module/platform/win32_library.h"
typedef anh::module::platform::Win32Library DynamicLibrary;
#else
#include "anh/module/platform/posix_library.h"
typedef anh::module::platform::PosixLibrary DynamicLibrary;
#endif

#if defined(__APPLE__)
  static std::string library_extension(".dylib");
#elif defined(WIN32)
  static std::string library_extension(".dll");
#else
  static std::string library_extension(".so");
#endif

using namespace anh::module;
using namespace std;

ModuleManager::ModuleManager() {
    app_binding_.version.major = 1;
    app_binding_.version.minor = 0;
    app_binding_.InvokeApi = nullptr;
    app_binding_.RegisterObject = std::bind(&ModuleManager::RegisterObject, this, placeholders::_1, placeholders::_2);
}

ModuleManager::~ModuleManager() {    
    auto end = exit_funcs_.end();
    std::for_each(exit_funcs_.begin(), exit_funcs_.end(), [] (ExitFunc exit_func) {
        try {
            (*exit_func)();
        } catch(...) {
            // Report error here.
        }
    });
}

bool ModuleManager::LoadModule(const std::string& path) {
    if (library_map_.find(path) != library_map_.end()) {
        cout << "Step 1 failed\n" << endl;
        return false;
    }

    auto library = LoadLibrary_(path);
    if (!library) {
        cout << "Step 3 failed\n" << endl;
        return false;
    }

    InitFunc init_func = library->GetSymbol<InitFunc>("InitializeModule");
    if (!init_func) {
        cout << "Step 4 failed\n" << endl;
        return false;
    }

    if (!InitializeModule(init_func)) {
        cout << "Step 5 failed\n" << endl;
        return false;
    }

    library_map_[path] = library;

    return true;
}

bool ModuleManager::LoadAllModules(const std::string& directory) {
    boost::filesystem::path module_dir_path(directory);

    bool all_loaded = true;

    try {
        if (!boost::filesystem::exists(module_dir_path) ||
            !boost::filesystem::is_directory(module_dir_path)) {
            all_loaded = false;
        }

        std::for_each(boost::filesystem::directory_iterator(module_dir_path),
            boost::filesystem::directory_iterator(),
            [this, &all_loaded] (const boost::filesystem::directory_entry& entry) 
        {            
            if (!this->LoadModule(entry.path().native_file_string())) {
                 all_loaded = false;
            }
        });

    } catch(const boost::filesystem::filesystem_error& /* e */) {
        // do some reporting;
        return false;
    }

    return all_loaded;
}

bool ModuleManager::InitializeModule(InitFunc init_func) {
    if (!init_func) {
        return false;
    }

    ExitFunc exit_func = init_func(&app_binding_);

    if (!exit_func) {
        return false;
    }

    exit_funcs_.push_back(exit_func);

    return true;
}

bool ModuleManager::RegisterObject(const std::string& name, const ObjectRegistration* registration) {
    if (name.empty()) {
        return false;
    }

    if (!registration || !registration->CreateObject || !registration->DestroyObject) {
        return false;
    }

    Version version = app_binding_.version;
    if (version.major != registration->version.major) {
        return false;
    }

    if (registration_map_.find(name) != registration_map_.end()) {
        return false;
    }

    registration_map_[name] = *registration;

    return true;
}

const RegistrationMap& ModuleManager::registration_map() {
    return registration_map_;
}

AppBinding& ModuleManager::app_binding() {
    return app_binding_;
}

std::shared_ptr<platform::LibraryInterface> ModuleManager::LoadLibrary_(const std::string& path) {
    auto library = DynamicLibrary::Load(path);

    if (!library) {
        return nullptr;
    }

    return library;
}
