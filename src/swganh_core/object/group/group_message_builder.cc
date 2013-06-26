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
        group->SerializeGroupMembers(&message);
        group->AddDeltasUpdate(&message);
    }
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

boost::optional<BaselinesMessage> GroupMessageBuilder::BuildBaseline3(const std::shared_ptr<Group>& group, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(group, lock, Object::VIEW_3, 8);
	message.data.append((*ObjectMessageBuilder::BuildBaseline3(group, lock)).data);
	return BaselinesMessage(std::move(message));
}

boost::optional<BaselinesMessage> GroupMessageBuilder::BuildBaseline6(const std::shared_ptr<Group>& group, boost::unique_lock<boost::mutex>& lock)
{
	auto message = CreateBaselinesMessage(group, lock, Object::VIEW_6, 6);
	message.data.append((*ObjectMessageBuilder::BuildBaseline6(group, lock)).data);
    group->SerializeGroupMembers(&message, lock);
	
	//Unknown List
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);

	//Unknown String
	message.data.write<std::string>("");

    message.data.write<uint16_t>(group->GetDifficulty(lock));
	message.data.write<uint32_t>(4); //Unknown Int
    message.data.write<uint64_t>(group->GetLootMaster(lock));
    message.data.write<uint32_t>(group->GetLootMode(lock));
	return BaselinesMessage(std::move(message));
}

}} // swganh::object