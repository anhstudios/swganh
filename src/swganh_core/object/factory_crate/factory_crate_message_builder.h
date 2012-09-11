// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_FACTORY_CRATE_FACTORY_CRATE_MESSAGE_BUILDER_H_

#include "swganh_core/object/tangible/tangible_message_builder.h"

namespace swganh {
namespace object {


    class FactoryCrate;
    class FactoryCrateMessageBuilder : public swganh::object::TangibleMessageBuilder
    {
    public:
        FactoryCrateMessageBuilder(swganh::EventDispatcher* dispatcher) :
            TangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        virtual void SendBaselines(const std::shared_ptr<FactoryCrate>& factory_crate, const std::shared_ptr<swganh::observer::ObserverInterface>& controller);
        
        // baselines
        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<FactoryCrate>& factory_crate);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<FactoryCrate>& factory_crate);
    private:
        typedef swganh::ValueEvent<std::shared_ptr<FactoryCrate>> FactoryCrateEvent;
    };

}}  // swganh::object

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_
