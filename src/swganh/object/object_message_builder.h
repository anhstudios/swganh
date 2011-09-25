
#ifndef SWGANH_OBJECT_OBJECT_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_OBJECT_MESSAGE_BUILDER_H_

#include <memory>
#include <boost/optional.hpp>

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {

    class Object;

    class ObjectMessageBuilder
    {
    public:
        static void BuildComplexityDelta(Object* object);
        static void BuildStfNameDelta(Object* object);
        static void BuildCustomNameDelta(Object* object);
        static void BuildVolumeDelta(Object* object);
        // delta 6
        static void BuildServerIDDelta(Object* object);

        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(Object* object);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(Object* object);
    };

}}  // swganh::object

#endif  // SWGANH_OBJECT_OBJECT_MESSAGE_BUILDER_H_
