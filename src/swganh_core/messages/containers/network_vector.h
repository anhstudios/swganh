// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <vector>
#include <queue>
#include <functional>

#include "detault_serializer.h"

namespace swganh
{
namespace containers
{

template<typename T, typename Serializer=DefaultSerializer<T>>
class NetworkVector
{
public:

	void remove(uint16_t index, bool update=true)
	{
		data_.remove(index);
		if(update)
		{
			deltas_.push_back([=] (swganh::messages::DeltasMessage& message) {
				message.data.write<uint8_t>(0);
				message.data.write<uint16_t>(index);
			});
		}
	}
	
	void add(T& data, bool update=true)
	{
		data_.push_back(data);
		
		uint16_t index = data_.size()-1;
		T& new_data = data_[index];
		
		if(update)
		{
			deltas_.push_back([=] (swganh::messages::DeltasMessage& message) {
				message.data.write<uint8_t>(1);
				message.data.write<uint16_t>(index);
				Serializer::SerializeDelta(message, new_data);
			});
		}
	}
	
	void update(uint16_t index)
	{
		deltas_.push_back([=] (swganh::messages::DeltasMessage& message) {
			message.data.write<uint8_t>(2);
			Serializer::SerializeDelta(message, data_[index]);
		});
	}
	
	void reset(std::vector<T> other, bool update=true)
	{
		if(update)
		{
			//A copy will be made for the update
			deltas_.push_back([=] (swganh::messages::DeltasMessage& message) {
				message.data.write<uint8_t>(3);
				message.data.write<uint16_t>(other.size());
			});
		}
		
		//The original will be moved in.
		data_ = std::move(other);
	}
	
	void clear(bool update=true)
	{
		data_.clear();
		if(update)
		{
			deltas_.push_back([] (swganh::messages::DeltasMessage& message) {
				message.data.write<uint8_t>(4);
			});
		}
	}

	std::vector<T> data()
	{
		return data_;
	}
	
	std::vector<T>& raw()
	{
		return data_;
	}
	
	T& get(const uint16_t index)
	{
		return data_.get(index);
	}
	
	T& operator[](const uint16_t index)
	{
		return data_.get(index);
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
			deltas_.top()(message.data);
			deltas_.pop();
		}
	}
	
private:
	std::vector<T> data_;
	uint32_t update_counter_;
	std::queue<std::function<void(swganh::messages::DeltasMessage&)>> deltas_;
};

}
}