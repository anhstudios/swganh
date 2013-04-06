// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "harvester_installation_message_builder.h"

#include "harvester_installation.h"
#include "swganh_core/object/object_events.h"
#include "swganh_core/messages/scene_end_baselines.h"
#include "swganh_core/messages/deltas_message.h"
#include "swganh_core/messages/baselines_message.h"

using namespace swganh;
using namespace std;
using namespace swganh::object;
using namespace swganh::object;
using namespace swganh::messages;

void HarvesterInstallationMessageBuilder::RegisterEventHandlers()
{
}

void HarvesterInstallationMessageBuilder::SendBaselines(const shared_ptr<HarvesterInstallation>& harvester_installation, const shared_ptr<swganh::observer::ObserverInterface>& observer)
{
    auto baseline3 = BuildBaseline3(harvester_installation);
    auto baseline6 = BuildBaseline6(harvester_installation);
    auto baseline7 = BuildBaseline7(harvester_installation);
    
    observer->Notify(&baseline3);
    observer->Notify(&baseline6);
	observer->Notify(&baseline7);
        
    SendEndBaselines(harvester_installation, observer);
}

// baselines
BaselinesMessage HarvesterInstallationMessageBuilder::BuildBaseline3(const shared_ptr<HarvesterInstallation>& harvester_installation)
{
    auto message = CreateBaselinesMessage(harvester_installation, Object::VIEW_3, 11);
    message.data.append(InstallationMessageBuilder::BuildBaseline3(harvester_installation).data);
    return BaselinesMessage(std::move(message));
}

BaselinesMessage HarvesterInstallationMessageBuilder::BuildBaseline6(const shared_ptr<HarvesterInstallation>& harvester_installation)
{
    auto message = CreateBaselinesMessage(harvester_installation, Object::VIEW_6, 2);
    message.data.append(InstallationMessageBuilder::BuildBaseline6(harvester_installation).data);
    return BaselinesMessage(std::move(message));
}

BaselinesMessage HarvesterInstallationMessageBuilder::BuildBaseline7(const shared_ptr<HarvesterInstallation>& harvester_installation)
{
	auto message = CreateBaselinesMessage(harvester_installation, Object::VIEW_7, 16);
	message.data.append(InstallationMessageBuilder::BuildBaseline7(harvester_installation).data);
	return BaselinesMessage(std::move(message));
}