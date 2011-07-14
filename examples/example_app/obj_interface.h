
#ifndef EXAMPLE_APP_OBJ_INTERFACE_H_
#define EXAMPLE_APP_OBJ_INTERFACE_H_

#include <string>

class ObjInterface {
public:
    virtual ~ObjInterface() {}

    virtual std::string DoWork() = 0;
};

#endif  // EXAMPLE_APP_OBJ_INTERFACE_H_
