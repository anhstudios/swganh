#ifndef LIBANH_SCRIPTING_TEST_MODULES_H_
#define LIBANH_SCRIPTING_TEST_MODULES_H_

#include <boost/python.hpp>
#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>
#include <anh/component/mock_component.h>

using namespace anh::component;
namespace bp = boost::python;
#pragma warning( disable : 4172)

// An abstract base class
class Base
{
public:
  virtual ~Base() {};
  virtual std::string hello() = 0;
};
struct BaseCallback : Base {
public:
    BaseCallback(PyObject* p) : self(p) {}
    std::string hello() { return bp::call_method<std::string>(self, "hello");  } 
private:
    PyObject* self;
};
// C++ derived class
class CppDerived : public Base
{
public:
    virtual ~CppDerived() {}
    virtual std::string hello() { return "Hello from C++!";}
};

// Familiar Boost.Python wrapper class for Base
struct BaseWrap : Base, bp::wrapper<Base>
{
public:
  std::string hello() 
  {
    return this->get_override("hello")().as<std::string>();
  }
  
};
struct Y 
{
    Y(int z) : x(z) {}
    int x;
};
struct nonConstRef 
{
    nonConstRef(int z) : x(z) {}
    int x;
};
class GameObject
{
public:
    GameObject(int x_, int y_, float z_)
    : x(x_), y(y_), z(z_)
    {}
    int x;
    int y;
    float z;
    
};

const Y& f(int z) { return Y(z); }

void testPolicies() {
    bp::class_<Y>("Y", bp::init<int>())
        .def_readwrite("x", &Y::x);
    def("f", &f, bp::return_internal_reference<1>());
    bp::class_<nonConstRef>("nonConstRef", bp::init<int>())
        .def_readwrite("x", &nonConstRef::x);
    bp::class_<GameObject>("GameObject", bp::init<int, int, float>())
            .def_readwrite("x", &GameObject::x)
            .def_readwrite("y", &GameObject::y)
            .def_readwrite("z", &GameObject::z);
}
void baseDerive() { 
  /*  bp::class_<BaseWrap, boost::noncopyable>("Base")
        .def("hello", bp::pure_virtual(&Base::hello));*/
    bp::class_<Base, boost::noncopyable, boost::shared_ptr<BaseCallback>>("Base")
        .def("hello", &Base::hello);
}

///COMPONENT MODULE START
namespace anh { namespace component {
// Familiar Boost.Python wrapper class for ComponentInterface
// template magic!
template<class DerivedT> class BaseComponentWrap : public DerivedT, public bp::wrapper<DerivedT>
{
public:
    BaseComponentWrap(){ }
    virtual void Init(boost::property_tree::ptree& pt){
        return (void)this->get_override("Init")(pt);
    }
    virtual void Deinit(void) {
        return (void)this->get_override("Deinit")();
    }
    virtual void Update(const float timeout) {
        return (void)this->get_override("Update")();
    }
    virtual void HandleMessage(const Message message) {
        return (void)this->get_override("HandleMessage")(message);
    }
    virtual const ComponentType& component_type(void) {
        return this->get_override("component_type")();
    }
};

class MockComponentWrap : public BaseComponentWrap<MockComponent>
{
public:
    MockComponentWrap(){}
};

using boost::property_tree::ptree;

void componentDerive() { 
    bp::class_<MockComponentWrap, boost::noncopyable> MockComponentInterface();
    bp::class_<ptree>("ptree", bp::init<>());
}

} // component
} // anh
#endif //LIBANH_SCRIPTING_TEST_MODULES_H_
#pragma warning( default : 4172)
