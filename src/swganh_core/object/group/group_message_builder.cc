// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "group_message_builder.h"

#include "group.h"
#include "swganh_core/object/tangible/tangible.h"

using namespace std;
using namespace swganh::object;
using namespace swganh::messages;

namespace swganh {
namespace object {


void GroupMessageBuilder::RegisterEventHandlers()
{
    // TODO: Register Handlers for Group
}

void GroupMessageBuilder::BuildMemberListDelta(const shared_ptr<Group>& group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 1);
        group->GetGroupMembers().Serialize(message);
        group->AddDeltasUpdate(&message);
    }
    else
        group->GetGroupMembers().ClearDeltas();
}

void GroupMessageBuilder::BuildLootModeDelta(const shared_ptr<Group>& group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 7);
        message.data.write<uint32_t>(group->GetLootMode());
        group->AddDeltasUpdate(&message);
    }
}

void GroupMessageBuilder::BuildDifficultyDelta(const shared_ptr<Group>& group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 4);
        message.data.write<uint16_t>(group->GetDifficulty());
        group->AddDeltasUpdate(&message);
    }
}

void GroupMessageBuilder::BuildLootMasterDelta(const shared_ptr<Group>& group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 6);
        message.data.write<uint64_t>(group->GetLootMaster());
        group->AddDeltasUpdate(&message);
    }
}

void GroupMessageBuilder::SendBaselines(const std::shared_ptr<Group>& group, const std::shared_ptr<swganh::observer::ObserverInterface>& observer)
{
	group->AddBaselineToCache(&BuildBaseline3(group));
    group->AddBaselineToCache(&BuildBaseline6(group));

	for (auto& baseline : group->GetBaselines())
    {
        observer->Notify(&baseline);
    }
        
    SendEndBaselines(group, observer);
}

BaselinesMessage GroupMessageBuilder::BuildBaseline3(const std::shared_ptr<Group>& group)
{
	auto message = CreateBaselinesMessage(group, Object::VIEW_3, 8);
	message.data.append(ObjectMessageBuilder::BuildBaseline3(group).data);
	return BaselinesMessage(std::move(message));
}

BaselinesMessage GroupMessageBuilder::BuildBaseline6(const std::shared_ptr<Group>& group)
{
	auto message = CreateBaselinesMessage(group, Object::VIEW_6, 6);
	message.data.append(ObjectMessageBuilder::BuildBaseline6(group).data);
    group->GetGroupMembers().Serialize(message);
	
	//Unknown List
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);

	//Unknown String
	message.data.write<std::string>("");

    message.data.write<uint16_t>(group->GetDifficulty());
	message.data.write<uint32_t>(4); //Unknown Int
    message.data.write<uint64_t>(group->GetLootMaster());
    message.data.write<uint32_t>(group->GetLootMode());
	return BaselinesMessage(std::move(message));
}

}} // swganh::object