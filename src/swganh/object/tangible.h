
#ifndef SWGANH_OBJECT_TANGIBLE_H_
#define SWGANH_OBJECT_TANGIBLE_H_

#include <set>
#include "swganh/object/base_object.h"
#include "swganh/object/base_tangible.h"

namespace swganh {
namespace object {

class Tangible : public BaseTangible
{
public:
    Tangible();

    // TANO
    virtual uint32_t GetType() { return Tangible::type; }
    const static uint32_t type = 0x54414e4f;

private:
    virtual void OnReliableUpdate() {}
};
    
}}  // namespace

#endif  // SWGANH_OBJECT_TANGIBLE_H_
