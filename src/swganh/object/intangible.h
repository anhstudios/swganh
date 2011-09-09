
#ifndef SWGANH_OBJECT_INTANGIBLE_H_
#define SWGANH_OBJECT_INTANGIBLE_H_

#include "swganh/object/base_object.h"

namespace swganh {
namespace object {
    
class Intangible : public BaseObject
{
    std::string stf_detail_file;
    std::string stf_detail_name;
};
    
}}  // namespace

#endif  // SWGANH_OBJECT_BASE_OBJECT_H_
