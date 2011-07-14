
#ifndef EXAMPLE_MODULE_TEST_OBJ_H_
#define EXAMPLE_MODULE_TEST_OBJ_H_

#include <string>

#include "example_app/obj_interface.h"

class TestObj : public ObjInterface {
public:
    std::string DoWork();
};

#endif  // EXAMPLE_MODULE_TEST_OBJ_H_
