
#ifndef ANH_MODULE_BINDINGS_H_
#define ANH_MODULE_BINDINGS_H_

#include <cstdint>
#include <functional>
#include <string>

namespace anh {
namespace module {

struct AppBinding;

struct Version {
    int32_t major;
    int32_t minor;

    std::string ToString() {
        return major + "." + minor;
    }
};

struct ObjectParams {
    std::string name;
    const struct AppBinding * app_binding;
};

typedef std::function<void * (ObjectParams*)> ObjectCreator;
typedef std::function<void (void *)> ObjectDestroyer;

struct ObjectRegistration {
    Version version;
    ObjectCreator CreateObject;
    ObjectDestroyer DestroyObject;
};

typedef std::function<bool (const std::string&, const ObjectRegistration*)> ObjectRegistrar;
typedef std::function<bool (const std::string&, void *)> ApiInvoker;

struct AppBinding {
    Version version;
    ObjectRegistrar RegisterObject;
    ApiInvoker InvokeApi;
};

typedef void (*ExitFunc)();
typedef ExitFunc (*InitFunc)(const AppBinding *);

#ifdef WIN32
    #ifdef DLL_EXPORTS
        #define MODULE_API __declspec(dllexport)
    #else
        #define MODULE_API __declspec(dllimport)
    #endif
#else
    #define MODULE_API 
#endif

extern
#ifdef __cplusplus
    "C"
#endif
MODULE_API ExitFunc InitializeModule(const AppBinding* binding);

}}  // namespace anh::modules

#endif  // ANH_MODULE_BINDINGS_H_
