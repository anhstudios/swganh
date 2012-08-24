#include "installation_message_builder.h"

#include "installation.h"
#include "swganh/object/object_events.h"
#include "pub14_core/messages/scene_end_baselines.h"
#include "pub14_core/messages/deltas_message.h"
#include "pub14_core/messages/baselines_message.h"

using namespace anh;
using namespace std;
using namespace swganh::object;
using namespace swganh::object::installation;
using namespace swganh::messages;

void InstallationMessageBuilder::RegisterEventHandlers()
{
}

void InstallationMessageBuilder::SendBaselines(const std::shared_ptr<Installation>& installation, const std::shared_ptr<anh::observer::ObserverInterface>& observer)
{
	installation->AddBaselineToCache(BuildBaseline3(installation));
    installation->AddBaselineToCache(BuildBaseline6(installation));
    installation->AddBaselineToCache(BuildBaseline7(installation));
    
    for (auto& baseline : installation->GetBaselines())
    {
        observer->Notify(baseline);
    }
        
    SendEndBaselines(installation, observer);
}

void InstallationMessageBuilder::BuildActiveDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildPowerReserveDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildPowerCostDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildAvailableResourceDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildDisplayedMaxExtractionDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildMaxExtractionDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildCurrentExtractionDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildCurrentHopperSizeDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildMaxHopperSizeDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildIsUpdatingDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildHopperDelta(const std::shared_ptr<Installation>& installation)
{
}

void InstallationMessageBuilder::BuildConditionPercentDelta(const std::shared_ptr<Installation>& installation)
{
}

BaselinesMessage InstallationMessageBuilder::BuildBaseline3(const std::shared_ptr<Installation>& installation)
{
	auto message = CreateBaselinesMessage(installation, Object::VIEW_3, 14);
    message.data.append(TangibleMessageBuilder::BuildBaseline3(installation).data);
	message.data.write<uint8_t>((installation->IsActive()) ? 1 : 0);
	message.data.write(installation->GetPowerReserve());
	message.data.write(installation->GetPowerCost());
    return BaselinesMessage(std::move(message));
}

BaselinesMessage InstallationMessageBuilder::BuildBaseline6(const std::shared_ptr<Installation>& installation)
{
	auto message = CreateBaselinesMessage(installation, Object::VIEW_6, 2);
    message.data.append(TangibleMessageBuilder::BuildBaseline6(installation).data);
    return BaselinesMessage(std::move(message));
}

BaselinesMessage InstallationMessageBuilder::BuildBaseline7(const std::shared_ptr<Installation>& installation)
{
	auto message = CreateBaselinesMessage(installation, Object::VIEW_7, 11);
    message.data.append(TangibleMessageBuilder::BuildBaseline7(installation).data);
	installation->GetResourceIds_.Serialize(message);
	installation->GetResourceNames_.Serialize(message);
	installation->GetResourceTypes_.Serialize(message);
	message.data.write(installation->GetSelectedResourceId());
	message.data.write<uint8_t>((installation->IsActive()) ? 1 : 0);
	message.data.write(installation->GetDisplayedCurrentExtractionRate());
	message.data.write(installation->GetMaxExtractionRate());
	message.data.write(installation->GetCurrentExtractionRate());
	message.data.write(installation->GetCurrentHopperSize());
	message.data.write(installation->GetMaxHopperSize());
	message.data.write<uint8_t>((installation->IsUpdating()) ? 1 : 0);
	installation->GetHopperContents().Serialize(message);
	message.data.write(installation->GetConditionPercentage());
    return BaselinesMessage(std::move(message));
}
