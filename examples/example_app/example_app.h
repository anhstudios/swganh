
#ifndef EXAMPLE_APP_EXAMPLE_APP_H_
#define EXAMPLE_APP_EXAMPLE_APP_H_

#include "anh/module/module_manager.h"

class ExampleApp {
public:
    void Start();

private:
    anh::module::ModuleManager module_manager_;
};

#endif  // EXAMPLE_APP_EXAMPLE_APP_H_
