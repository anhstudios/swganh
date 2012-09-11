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
    harvester_installation->AddBaselineToCache(&BuildBaseline3(harvester_installation));
    harvester_installation->AddBaselineToCache(&BuildBaseline6(harvester_installation));
	harvester_installation->AddBaselineToCache(&BuildBaseline7(harvester_installation));

    for (auto& baseline : harvester_installation->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
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