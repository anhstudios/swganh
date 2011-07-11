
#include "example_app/example_app.h"

#include <iostream>

#include "example_app/obj_interface.h"

void ExampleApp::Start() {
    if (!module_manager_.LoadModule("mod_example_module")) {
        std::cout << "Failed to load module :(\n" << std::endl;
        return;
    }

    std::shared_ptr<ObjInterface> my_obj = module_manager_.CreateObject<ObjInterface>("TestObj");

    if (!my_obj) {
        std::cout << "Failed to create object :(\n" << std::endl;
    } else {
        std::cout << my_obj->DoWork() << std::endl;
    }

    std::shared_ptr<ObjInterface> python_obj = module_manager_.CreateObject<ObjInterface>("PythonObj");

    if (!python_obj) {
        std::cout << "Failed to create object :(\n" << std::endl;
    } else {
        std::cout << python_obj->DoWork() << std::endl;
    }
}
