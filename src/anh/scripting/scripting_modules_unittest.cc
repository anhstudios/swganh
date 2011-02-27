#include <boost/python.hpp>
#include <anh/component/component_interface.h>
#include <anh/component/base_component.h>
#include <anh/component/test_components_unittest.h>

namespace bp = boost::python;
using namespace anh::test_components;
#pragma warning( disable : 4172)


// An abstract base class
class Base
{
public:
  virtual ~Base() {};
  virtual std::string hello() = 0;
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

Y& f(int z) { return Y(z); }

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
    bp::class_<BaseWrap, boost::noncopyable>("Base")
        .def("hello", bp::pure_virtual(&Base::hello));
}

///COMPONENT MODULE START
namespace anh { namespace component {

// Familiar Boost.Python wrapper class for ComponentInterface
// template magic!
template<class DerivedT> class BaseComponentWrap : public DerivedT, public bp::wrapper<DerivedT>
{
public:
    BaseComponentWrap(ObjectId obj_id) : DerivedT(obj_id) { }
    virtual void Init(boost::property_tree::ptree& pt){
        return this->get_override("Init")(pt).as<void>();
    }
    virtual void Deinit(void) {
        return this->get_override("Deinit")().as<void>();
    }
    virtual void Update(const float timeout) {
        return this->get_override("Update")().as<void>();
    }
    virtual void HandleMessage(const Message message) {
        return this->get_override("HandleMessage")(message).as<void>();
    }
    virtual const ComponentInfo& component_info(void) {
        return this->get_override("component_info")();
    }
};
class BaseComponentWrapped : public BaseComponentWrap<BaseComponent>
{
public:
    BaseComponentWrapped() : BaseComponentWrap(0) {}
};
// HAM
class HAMComponentWrap : public BaseComponentWrap<HAMComponentInterface>
{   
public:

    HAMComponentWrap() : BaseComponentWrap(0) {}
    
    virtual HAM& ham(void) {
       return this->get_override("ham")().as<HAM>();
    }
};
class RadialComponentWrap : public BaseComponentWrap<RadialComponentInterface>
{
public:
    RadialComponentWrap() : BaseComponentWrap(0) {}
};

using boost::property_tree::ptree;

void componentDerive() { 
    bp::class_<BaseComponentWrapped, boost::noncopyable> BaseComponent("BaseComponent");
    bp::class_<HAMComponentWrap, boost::noncopyable> HAMComponentInterface("HAMComponentInterface");
    bp::class_<HAM> ("HAM")
        .def_readwrite("health", &HAM::health)
        .def_readwrite("max_health", &HAM::max_health)
        .def_readwrite("action", &HAM::action)
        .def_readwrite("max_action", &HAM::max_action)
        .def_readwrite("mind", &HAM::max_mind)
        .def_readwrite("max_mind", &HAM::max_mind);

    bp::class_<RadialComponentWrap, boost::noncopyable> RadialComponentInterface("RadialComponentInterface");
    bp::class_<ptree>("ptree", bp::init<>());
}

}
}
#pragma warning( default : 4172)