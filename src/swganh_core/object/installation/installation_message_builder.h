// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_INSTALLATION_INSTALLATION_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_INSTALLATION_INSTALLATION_MESSAGE_BUILDER_H_

#include "swganh_core/object/tangible/tangible_message_builder.h"

namespace swganh {
namespace object {


    class Installation;
    class InstallationMessageBuilder : public swganh::object::TangibleMessageBuilder
    {
    public:
        InstallationMessageBuilder(swganh::EventDispatcher* dispatcher) :
            TangibleMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
		
		virtual ~InstallationMessageBuilder() {}

        // deltas
        static void BuildActiveDelta(const std::shared_ptr<Installation>& installation);
		static void BuildPowerReserveDelta(const std::shared_ptr<Installation>& installation);
        static void BuildPowerCostDelta(const std::shared_ptr<Installation>& installation);
        static void BuildAvailableResourceDelta(const std::shared_ptr<Installation>& installation);
		static void BuildSelectedResourceDelta(const std::shared_ptr<Installation>& installation);
		static void BuildDisplayedMaxExtractionDelta(const std::shared_ptr<Installation>& installation);
		static void BuildMaxExtractionDelta(const std::shared_ptr<Installation>& installation);
		static void BuildCurrentExtractionDelta(const std::shared_ptr<Installation>& installation);
		static void BuildCurrentHopperSizeDelta(const std::shared_ptr<Installation>& installation);
		static void BuildMaxHopperSizeDelta(const std::shared_ptr<Installation>& installation);
		static void BuildIsUpdatingDelta(const std::shared_ptr<Installation>& installation);
		static void BuildHopperDelta(const std::shared_ptr<Installation>& installation);
		static void BuildConditionPercentDelta(const std::shared_ptr<Installation>& installation);

        // baselines
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline3(const std::shared_ptr<Installation>& installation, boost::unique_lock<boost::mutex>& lock);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline6(const std::shared_ptr<Installation>& installation, boost::unique_lock<boost::mutex>& lock);
        static boost::optional<swganh::messages::BaselinesMessage> BuildBaseline7(const std::shared_ptr<Installation>& installation, boost::unique_lock<boost::mutex>& lock);
    private:
        typedef swganh::ValueEvent<std::shared_ptr<Installation>> InstallationEvent;
        
        void RegisterEventHandlers();
    };

}}  // swganh::object

#endif  // SWGANH_OBJECT_TANGIBLE_TANGIBLE_MESSAGE_BUILDER_H_
