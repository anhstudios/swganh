// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <queue>
#include <functional>

#include "default_serializer.h"

namespace swganh
{
namespace containers
{

template<typename K, typename V, typename Serializer=DefaultSerializer<T>>
class NetworkMap
{
public:

	void remove(const K& key, bool update=true)
	{
		auto itr = data_.find(key);
		if(itr != data_.end())
		{
			if(update)
			{
				deltas_.push([=] (swganh::messages::DeltasMessage& message) {
					message.data.write<uint8_t>(1);
					Serializer::SerializeDelta(message.data, itr.second);
				});
			}
			data_.erase(itr);
		}
	}
	
	void add(const K& key, const V& value, bool update=true)
	{
		auto pair = data_.insert(std::make_pair(key, value));
		if(pair.second)
		{
			if(update)
			{
				deltas_.push([=] (swganh::messages::DeltasMessage& message) {
					message.data.write<uint8_t>(1);
					Serializer::SerializeDelta(message.data, pair.first->second);
				});
			}
		}
	}

	void update(const K& key)
	{
		deltas_.push([=] (swganh::messages::DeltasMessage& message) {
			message.data.write<uint8_t>(2);
			Serializer::SerializeDelta(message.data, data_[key]);
		});
	}

	std::set<T> data()
	{
		return data_;
	}
	
	std::set<T>& raw()
	{
		return data_;
	}

	uint32_t size()
	{
		return data_.size();
	}

	void Serialize(swganh::messages::BaseSwgMessage* message)
	{
		if(message->Opcode() == swganh::messages::BaselinesMessage::opcode)
		{
			Serialize(*((swganh::messages::BaselinesMessage*)message));
		}
		else if(message->Opcode() == swganh::messages::DeltasMessage::opcode)
		{
			Serialize(*((swganh::messages::DeltasMessage*)message));
		}
	}
	
	void Serialize(swganh::messages::BaselinesMessage& message)
	{
		message.data.write<uint32_t>(data_.size());
        message.data.write<uint32_t>(0);
		for(auto& item : data_)
		{
			Serializer::SerializeBaseline(message, item);
		}
	}
	
	void Serialize(swganh::messages::DeltasMessage& message)
	{
		message.data.write<uint32_t>(deltas_.size());
		message.data.write<uint32_t>(++update_counter_);
		
		while(!deltas_.empty())
		{
			deltas_.top()(message);
			deltas_.pop();
		}
	}

private:
	std::map<K, V> data_;

	uint32_t update_counter_;
	std::queue<std::function<void(swganh::messages::DeltasMessage&)>> deltas_;
};

}
}