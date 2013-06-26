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
{}

// baselines
boost::optional<BaselinesMessage> HarvesterInstallationMessageBuilder::BuildBaseline3(const shared_ptr<HarvesterInstallation>& harvester_installation, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(harvester_installation, lock, Object::VIEW_3, 11);
    message.data.append((*InstallationMessageBuilder::BuildBaseline3(harvester_installation, lock)).data);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> HarvesterInstallationMessageBuilder::BuildBaseline6(const shared_ptr<HarvesterInstallation>& harvester_installation, boost::unique_lock<boost::mutex>& lock)
{
    auto message = CreateBaselinesMessage(harvester_installation, lock, Object::VIEW_6, 2);
    message.data.append((*InstallationMessageBuilder::BuildBaseline6(harvester_installation, lock)).data);
    return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> HarvesterInstallationMessageBuilder::BuildBaseline7(const shared_ptr<HarvesterInstallation>& harvester_installation, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(harvester_installation, lock, Object::VIEW_7, 16);
	message.data.append((*InstallationMessageBuilder::BuildBaseline7(harvester_installation, lock)).data);
	return BaselinesMessage(std::move(message));
}
