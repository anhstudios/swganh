// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <set>
#include <queue>
#include <functional>

#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/deltas_message.h"

#include "default_serializer.h"

namespace swganh
{
namespace containers
{

template<typename T, typename Serializer=DefaultSerializer<T>>
class NetworkSet
{
public:
	typedef typename std::set<T>::const_iterator const_iterator;
	typedef typename std::set<T>::iterator iterator;
	
	NetworkSet()
		: update_counter_(0)
	{
	}

	void remove(const T& data, bool update=true)
	{
		remove(data_.find(data));
	}
	
	void remove(iterator itr, bool update=true)
	{
		if(itr != data_.end())
		{
			if(update)
			{
				const T& data = *itr;
				deltas_.push([=] (swganh::messages::DeltasMessage& message) {
					message.data.write<uint8_t>(0);
					Serializer::SerializeDelta(message.data, data);
				});
			}
			data_.erase(itr);
		}
	}

	void add(const T& data, bool update=true)
	{
		auto pair = data_.insert(data);
		if(pair.second)
		{
			if(update)
			{
				deltas_.push([=] (swganh::messages::DeltasMessage& message) {
					message.data.write<uint8_t>(1);
					Serializer::SerializeDelta(message.data, *pair.first);
				});
			}
		}
	}
	
	void clear(bool update=true)
	{
		data_.clear();
		if(update)
		{
			deltas_.push([=] (swganh::messages::DeltasMessage& message) {
				message.data.write<uint8_t>(2);
			});
		}
	}
	
	bool contains(const T& data)
	{
		return data_.find(data) != data_.end();
	}

	std::set<T> data()
	{
		return data_;
	}
	
	std::set<T>& raw()
	{
		return data_;
	}

	iterator begin()
	{
		return data_.begin();
	}

	iterator end()
	{
		return data_.end();
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
			Serializer::SerializeBaseline(message.data, item);
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
	std::set<T> data_;
	
	uint32_t update_counter_;
	std::queue<std::function<void(swganh::messages::DeltasMessage&)>> deltas_;
};

}
}