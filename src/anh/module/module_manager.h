
#ifndef ANH_MODULE_MODULE_MANAGER_H_
#define ANH_MODULE_MODULE_MANAGER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "anh/module/bindings.h"

namespace anh {
namespace module {

namespace platform {
class LibraryInterface;
}  // namespace platform

typedef std::map<std::string, ObjectRegistration> RegistrationMap;

class ModuleManager {
public:
    ModuleManager();
    ~ModuleManager();

    bool LoadModule(const std::string& path);
    bool LoadAllModules(const std::string& directory);

    bool InitializeModule(InitFunc init_func);

    bool RegisterObject(const std::string& name, const ObjectRegistration* registration);

    template<typename T>
    std::shared_ptr<T> CreateObject(const std::string& name);

    const RegistrationMap& registration_map();
    AppBinding& app_binding();

private:
    std::shared_ptr<platform::LibraryInterface> LoadLibrary_(const std::string& path);
    void * CreateObject_(const std::string& name);

private:
    typedef std::map<std::string, std::shared_ptr<platform::LibraryInterface>> LibraryMap;
    typedef std::vector<ExitFunc> ExitFuncCollection;

    AppBinding app_binding_;
    LibraryMap library_map_;
    ExitFuncCollection exit_funcs_;

    RegistrationMap registration_map_;
};

template<typename T>
std::shared_ptr<T> ModuleManager::CreateObject(const std::string& name) {    
    auto find_iter = registration_map_.find(name);
    std::shared_ptr<T> object = nullptr;

    if (find_iter != registration_map_.end()) {
        auto registration = find_iter->second;

        ObjectParams params;
        params.name = name;
        params.app_binding = &app_binding_;

        object = std::shared_ptr<T>(static_cast<T*>(registration.CreateObject(&params)), registration.DestroyObject);
    }

    return object;
}

}}  // namespace anh::module

#endif  // ANH_MODULE_MODULE_MANAGER_H_
