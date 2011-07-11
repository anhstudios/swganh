
#ifndef ANH_MODULE_MODULE_MANAGER_H_
#define ANH_MODULE_MODULE_MANAGER_H_

namespace anh {
namespace module {

class ModuleManagerInterface {
public:
    virtual ~ModuleManagerInterface() {}

    virtual bool LoadModule(const std::string& path);
    virtual bool LoadAllModules(const std::string& directory);

    virtual bool InitializeModule(InitFunc init_func);

    virtual bool RegisterObject(const std::string& name, const ObjectRegistration* registration);

    template<typename T>
    std::shared_ptr<T> CreateObject(const std::string& name);

    const RegistrationMap& registration_map();
    AppBinding& app_binding();

};

}}  // namespace anh::module

#endif  // ANH_MODULE_MODULE_MANAGER_H_
