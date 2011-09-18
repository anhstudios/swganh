
#ifndef SWGANH_OBJECT_STATIC_H_
#define SWGANH_OBJECT_STATIC_H_

#include "base_object.h"


namespace swganh {
namespace object {
  
class Static

{
public:
    // STAO
    virtual uint32_t GetType() { return Static::type; }
    const static uint32_t type = 0x5354414F;

protected:
    virtual void OnReliableUpdate() = 0;
};

}}  // namespace

#endif  // SWGANH_OBJECT_STATIC_H_
