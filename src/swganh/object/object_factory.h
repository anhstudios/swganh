
#ifndef SWGANH_OBJECT_OBJECT_FACTORY_H_
#define SWGANH_OBJECT_OBJECT_FACTORY_H_

#include "object.h"


namespace swganh {
namespace object {

class ObjectFactory
{
public:
    ObjectFactory();
    virtual ~ObjectFactory(){}

    template<class ObjectType>
    static ObjectType CreateObject();

    template<class ObjectType>
    static ObjectType CreateObject(uint64_t id);

};
}}  // namespace swganh::object


#endif //SWGANH_OBJECT_OBJECT_FACTORY_H_