// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_HARVESTER_INSTALLATION_HARVESTER_INSTALLATION_MESSAGE_BUILDER_H_
#define SWGANH_OBJECT_HARVESTER_INSTALLATION_HARVESTER_INSTALLATION_MESSAGE_BUILDER_H_

#include "swganh/object/installation/installation_message_builder.h"

namespace swganh {
namespace object {
namespace harvester_installation {

    class HarvesterInstallation;
    class HarvesterInstallationMessageBuilder : public swganh::object::installation::InstallationMessageBuilder
    {
    public:
        HarvesterInstallationMessageBuilder(anh::EventDispatcher* dispatcher) :
            InstallationMessageBuilder(dispatcher)
        {
            RegisterEventHandlers();
        }
        virtual void RegisterEventHandlers();
        virtual void SendBaselines(const std::shared_ptr<HarvesterInstallation>& harvester_installation, const std::shared_ptr<anh::observer::ObserverInterface>& controller);
        
        // baselines
        static swganh::messages::BaselinesMessage BuildBaseline3(const std::shared_ptr<HarvesterInstallation>& harvester_installation);
        static swganh::messages::BaselinesMessage BuildBaseline6(const std::shared_ptr<HarvesterInstallation>& harvester_installation);
		static swganh::messages::BaselinesMessage BuildBaseline7(const std::shared_ptr<HarvesterInstallation>& harvester_installation);
    private:
        typedef anh::ValueEvent<std::shared_ptr<HarvesterInstallation>> HarvesterInstallationEvent;
    };

}}}  // swganh::object::tangible

#endif  // SWGANH_OBJECT_HARVESTER_INSTALLATION_HARVESTER_INSTALLATION_MESSAGE_BUILDER_H_
