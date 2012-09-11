// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_CELL_CELL_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_CELL_CELL_MESSAGE_BUILDER_H_

#include <memory>
#include <boost/optional.hpp>

#include "swganh_core/object/intangible/intangible_message_builder.h"

namespace swganh {
namespace messages {
    struct BaselinesMessage;
}} // swganh::messages

namespace swganh {
namespace object {


    class Cell;

    class CellMessageBuilder : public swganh::object::IntangibleMessageBuilder
    {
    public:
        CellMessageBuilder(swganh::EventDispatcher* dispatcher) :
            IntangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        static void BuildStfDetailDelta(const std::shared_ptr<Cell>& intangible);

		virtual void SendBaselines(const std::shared_ptr<Cell>& cell, const std::shared_ptr<swganh::observer::ObserverInterface>& observer);

		static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<Cell>& cell);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<Cell>& cell);
    };

}}  // swganh::object

#endif  // SWGANH_OBJECT_INTANGIBLE_INTANGIBLE_MESSAGE_BUILDER_H_
