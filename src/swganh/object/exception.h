
#ifndef SWGANH_OBJECT_EXCEPTION_H_
#define SWGANH_OBJECT_EXCEPTION_H_

#include <exception>

namespace swganh {
namespace object {
    
    typedef std::runtime_error InvalidObject;
    typedef std::runtime_error InvalidObjectTemplate;    
    typedef std::runtime_error InvalidObjectType;

}}  // namespace swganh::object

#endif  //SWGANH_OBJECT_EXCEPTION_H_
