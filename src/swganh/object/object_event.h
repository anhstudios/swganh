

#ifndef SWGANH_OBJECT_OBJECT_EVENT_H_
#define SWGANH_OBJECT_OBJECT_EVENT_H_

#include <cstdint>

#include "anh/event_dispatcher/basic_event.h"
#include "swganh/object/object.h"

namespace swganh {
namespace object {
    
    struct ObjectData
    {
        std::shared_ptr<Object> object;
    };

    typedef anh::event_dispatcher::BasicEvent<ObjectData> ObjectEvent;
    
}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_OBJECT_EVENT_H_

