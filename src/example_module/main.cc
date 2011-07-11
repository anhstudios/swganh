
#include "anh/module/bindings.h"

#include "example_app/python_obj.h"
#include "example_module/test_obj.h"

using namespace anh::module;

template<typename T>
class ObjectLifetimeWrapper {
public:
    static void* CreateObject(ObjectParams* params) {
        return new T();
    }

    static void DestroyObject(void * object) {
        if (object) {
            delete static_cast<T*>(object);
        }
    }
};

extern "C" MODULE_API void ExitModule() {
    return;
}

extern "C" MODULE_API ExitFunc InitializeModule(const AppBinding* app_binding) {

    ObjectRegistration registration;
    registration.version.major = 1;
    registration.version.minor = 0;

    // Register TestObj
    registration.CreateObject = &ObjectLifetimeWrapper<TestObj>::CreateObject;
    registration.DestroyObject = &ObjectLifetimeWrapper<TestObj>::DestroyObject;

    app_binding->RegisterObject("TestObj", &registration);
    
    // Register TestObj
    registration.CreateObject = [] (ObjectParams* params) {
        return new PythonObj("test_python_obj", "TestPythonObj");
    };

    registration.DestroyObject = [] (void * object) {
        if (object) {
            delete object;
        }
    };

    app_binding->RegisterObject("PythonObj", &registration);

    return ExitModule;
}
