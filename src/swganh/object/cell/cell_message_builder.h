// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_CELL_CELL_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_CELL_CELL_MESSAGE_BUILDER_H_

#include <memory>
#include <boost/optional.hpp>

#include "swganh/object/intangible/intangible_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {
namespace cell {

    class Cell;

    class CellMessageBuilder : public swganh::object::intangible::IntangibleMessageBuilder
    {
    public:
        CellMessageBuilder(anh::EventDispatcher* dispatcher) :
            IntangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        static void BuildStfDetailDelta(const std::shared_ptr<Cell>& intangible);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Intangible>& intangible);
    };

}}}  // swganh::object::intangible

#endif  // SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_
