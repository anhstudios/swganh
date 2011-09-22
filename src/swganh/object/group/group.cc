
#include "swganh/object/group/group.h"

#include "swganh/object/tangible/tangible.h"
#include "swganh/messages/deltas_message.h"

using namespace std;
using namespace swganh::messages;
using namespace swganh::object;
using namespace swganh::object::group;
using namespace swganh::object::tangible;

Group::Group()
    : member_list_counter_(0)
    , group_level_(0)
    , master_looter_(0)
    , loot_mode_(FREE_LOOT)
    , last_member_index_(0)
{
}

Group::~Group()
{
}

void Group::AddGroupMember(std::shared_ptr<BaseTangible> member)
{
    uint16_t next_index = GetNextMemberIndex_();
    member_list_.insert(std::make_pair(next_index, member));
    last_member_index_ = next_index;

    if(HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6);
        message.data.write<uint16_t>(1);
        message.data.write<uint32_t>(member_list_.size());
        message.data.write<uint32_t>(member_list_counter_++);
        message.data.write<uint8_t>(1);
        message.data.write<uint16_t>(next_index);
        message.data.write<uint64_t>(member->GetObjectId());
        message.data.write<std::string>(std::string(member->GetCustomName().begin(), member->GetCustomName().end()));
        
        AddDeltasUpdate(move(message));
    }
}

void Group::RemoveGroupMember(std::shared_ptr<BaseTangible> member)
{
    uint16_t index_position = 0;
    auto iter = std::find_if(member_list_.begin(), member_list_.end(), [=, &index_position](std::pair<uint16_t, std::shared_ptr<BaseTangible>> list_member)->bool {
        if(member->GetObjectId() == list_member.second->GetObjectId())
        {
            index_position = list_member.first;
            return true;
        }

        return false;
    });

    if(iter != member_list_.end())
    {
        member_list_.erase(iter);
        
        if(HasObservers())
        {
            DeltasMessage message = CreateDeltasMessage(Object::VIEW_6);
            message.data.write<uint16_t>(1);
            message.data.write<uint32_t>(member_list_.size());
            message.data.write<uint32_t>(member_list_counter_++);
            message.data.write<uint8_t>(0);
            message.data.write<uint16_t>(index_position);
        
            AddDeltasUpdate(move(message));
        }

        member_index_free_list_.push(index_position);
    }
}

void Group::SetLootMode(LOOT_MODE loot_mode)
{
    loot_mode_ = loot_mode;
        
    if(HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6);
        message.data.write<uint16_t>(7); // Update Type
        message.data.write<uint32_t>(loot_mode_);
        
        AddDeltasUpdate(move(message));
    }
}

void Group::SetGroupLevel(uint16_t group_level)
{
    group_level_ = group_level;
    
    if(HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6);
        message.data.write<uint16_t>(4); // Update Type
        message.data.write<uint16_t>(group_level_);
        
        AddDeltasUpdate(move(message));
    }
}

void Group::SetMasterLooter(uint64_t master_looter)
{
    master_looter_ = master_looter;
        
    if(HasObservers())
    {
        DeltasMessage message = CreateDeltasMessage(Object::VIEW_6);
        message.data.write<uint16_t>(6);
        message.data.write<uint64_t>(master_looter_);
        
        AddDeltasUpdate(move(message));
    }
}

boost::optional<BaselinesMessage> Group::GetBaseline6()
{
	auto message = CreateBaselinesMessage(Object::VIEW_6, 8);
	message.data.append(Object::GetBaseline6().get().data);
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(member_list_.size());
	message.data.write<uint32_t>(member_list_counter_);
	std::for_each(member_list_.begin(), member_list_.end(), [=, &message](std::pair<uint16_t, std::shared_ptr<BaseTangible>> list_member) {
		message.data.write<uint64_t>(list_member.second->GetObjectId());
		message.data.write<std::string>(std::string(list_member.second->GetCustomName().begin(), list_member.second->GetCustomName().end()));
	});
	message.data.write<uint32_t>(0);
	message.data.write<uint32_t>(0);
	message.data.write<std::string>("");
	message.data.write<uint16_t>(group_level_);
	message.data.write<uint32_t>(0);
	message.data.write<uint64_t>(master_looter_);
	message.data.write<uint32_t>(loot_mode_);
	return boost::optional<BaselinesMessage>(std::move(message));
}


uint16_t Group::GetNextMemberIndex_()
{
    uint16_t next_member_index = last_member_index_++;

    if(member_index_free_list_.size() > 0) {
        next_member_index = member_index_free_list_.top();
        member_index_free_list_.pop();
    }

    return next_member_index;
}
