// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <queue>
#include <functional>

#include "detault_serializer.h"

namespace swganh
{
namespace containers
{

template<typename K, typename V, Serializer=DefaultSerializer<T>>
class NetworkMap
{
public:

	void remove(K& key, bool update=true)
	{
		auto itr = data_.find(key);
		if(itr != data_.end())
		{
			if(update)
			{
				deltas_.push_back([=] (swganh::messages::DeltasMessage& message) {
					message.data.write<uint8_t>(1);
					Serializer::SerializeDelta(message, itr.second);
				});
			}
			data_.erase(itr);
		}
	}
	
	void add(K& key, V& value, bool update=true)
	{
		auto pair = data_.insert(std::make_pair(key, value));
		if(pair.second)
		{
			if(update)
			{
				deltas_.push_back([=] (swganh::messages::DeltasMessage& message) {
					message.data.write<uint8_t>(1);
					Serializer::SerializeDelta(message, pair.first.second);
				});
			}
		}
	}

	void update(K& key key)
	{
		deltas_.push_back([=] (swganh::messages::DeltasMessage& message) {
			message.data.write<uint8_t>(2);
			Serializer::SerializeDelta(message, data_[key]);
		});
	}

private:
	std::map<K, V> data_;

	uint32_t update_counter_;
	std::queue<std::function<void(swganh::messages::DeltasMessage&)>> deltas_;
};

}
}