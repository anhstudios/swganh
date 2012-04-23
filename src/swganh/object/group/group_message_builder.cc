// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "group_message_builder.h"

#include <swganh/object/group/group.h>
#include <swganh/object/tangible/tangible.h>

using namespace std;
using namespace swganh::object::group;
using namespace swganh::messages;

namespace swganh {
namespace object {
namespace group {

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
        group->AddDeltasUpdate(move(message));
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
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildDifficultyDelta(const shared_ptr<Group>& group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 4);
        message.data.write<uint16_t>(group->GetDifficulty());
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildLootMasterDelta(const shared_ptr<Group>& group)
{
    if(group->HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(group, Object::VIEW_6, 6);
        message.data.write<uint64_t>(group->GetLootMaster());
        group->AddDeltasUpdate(move(message));
    }
}

void GroupMessageBuilder::BuildBaseline3(const shared_ptr<Group>& group)
{
    auto message = CreateBaselinesMessage(group, Object::VIEW_3, 8);
	ObjectMessageBuilder::BuildBaseline3(group);
}

void GroupMessageBuilder::BuildBaseline6(const shared_ptr<Group>& group)
{
    auto message = CreateBaselinesMessage(group, Object::VIEW_6, 6);
	message.data.append(ObjectMessageBuilder::BuildBaseline6(group).data);
    group->GetGroupMembers().Serialize(message);
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);
	message.data.write<std::string>("");
    message.data.write<uint16_t>(group->GetDifficulty());
	message.data.write<uint32_t>(4);
    message.data.write<uint64_t>(group->GetLootMaster());
    message.data.write<uint32_t>(group->GetLootMode());
	group->AddBaselineToCache(message);
}

}}} // swganh::object::group