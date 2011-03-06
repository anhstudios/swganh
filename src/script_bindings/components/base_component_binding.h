#ifndef ZONE_BASE_COMPONENT_BINDING__H_
#define ZONE_BASE_COMPONENT_BINDING__H_

#include <boost/python.hpp>
#include <anh/component/base_component.h>

using namespace boost::python;
using namespace anh::component;

namespace anh_python { namespace components {
// Familiar Boost.Python wrapper class for ComponentInterface
// template magic!
template<class DerivedT> class BaseComponentBinding : public DerivedT, public wrapper<DerivedT>
{
public:
    BaseComponentBinding(ObjectId obj_id) : DerivedT(obj_id) { }
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
    virtual const ComponentInfo& component_info(void) {
        return this->get_override("component_info")();
    }
};

template<class DerivedT> class BaseBinding : public DerivedT, public wrapper<DerivedT>
{
    BaseBinding(ObjectId obj_id) : DerivedT(obj_id) { }
};
} // components
} // anh_python
#endif // ZONE_BASE_COMPONENT_BINDING__H_