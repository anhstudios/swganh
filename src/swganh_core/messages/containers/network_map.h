// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <queue>
#include <functional>

#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/deltas_message.h"

#include "default_serializer.h"

namespace swganh
{
namespace containers
{

template<typename K, typename V, typename Serializer=DefaultSerializer<V>>
class NetworkMap
{
public:
	typedef typename std::map<K, V>::const_iterator const_iterator;
	typedef typename std::map<K, V>::iterator iterator;

	NetworkMap()
		: update_counter_(0)
	{
	}

	void remove(const K& key, bool update=true)
	{
		remove(data_.find(key));
	}
	
	void remove(iterator itr, bool update=true)
	{
		if(itr != data_.end())
		{
			if(update)
			{
				deltas_.push([=] (swganh::messages::DeltasMessage& message) {
					message.data.write<uint8_t>(1);
					Serializer::SerializeDelta(message.data, itr->second);
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
					message.data.write<uint8_t>(0);
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

	bool contains(const K& key)
	{
		return data_.find(key) != data_.end();
	}
	
	iterator find(const K& key)
	{
		return data_.find(key);
	}

	std::map<K,V> data()
	{
		return data_;
	}
	
	std::map<K,V>& raw()
	{
		return data_;
	}

	uint32_t size()
	{
		return data_.size();
	}

	iterator begin()
	{
		return data_.begin();
	}

	iterator end()
	{
		return data_.end();
	}

	V& operator[](const K& key)
	{
		return find(key)->second;
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
		for(auto& pair : data_)
		{
			Serializer::SerializeBaseline(message.data, pair.second);
		}
	}
	
	void Serialize(swganh::messages::DeltasMessage& message)
	{
		message.data.write<uint32_t>(deltas_.size());
		message.data.write<uint32_t>(++update_counter_);
		
		while(!deltas_.empty())
		{
			deltas_.front()(message);
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