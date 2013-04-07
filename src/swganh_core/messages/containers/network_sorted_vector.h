// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <vector>
#include <boost/noncopyable.hpp>

#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/deltas_message.h"

namespace swganh {
namespace messages {
namespace containers {

/**
 *
 */
template<typename T>
class NetworkSortedVector
{
public:
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::iterator iterator;

    NetworkSortedVector(uint16_t capacity)
        : clear_(false)
        , reinstall_(false)
        , update_counter_(0)
    {
        items_.reserve(capacity);
    }
	
	NetworkSortedVector(std::vector<T> orig)
		: items_(orig.begin(), orig.end())
		, update_counter_(0)
		, clear_(false)
		, reinstall_(false)
	{
	}

    ~NetworkSortedVector(void)
    {}

	std::vector<T> Get() const
	{
		return items_;
	}

    void Add(const T& item)
    {
		auto iter = std::find_if(items_.begin(), items_.end(), [=](T& i) {
            return (i == item);
        });

        if(iter == items_.end())
        {
            items_.push_back(item);
            items_added_.push_back(items_.size() - 1);
        }
    }

	void Remove(uint16_t index)
	{
		items_removed_.push_back(index);
		items_.erase(items_.begin()+index);
	}

    void Remove(iterator iter)
    {
		uint16_t distance = std::distance<const_iterator>(items_.begin(), iter);
        items_removed_.push_back(distance);
        items_.erase(iter);
    }

    void Insert(const T& item)
    {
		auto iter = std::find_if(items_.begin(), items_.end(), [=](T& i) {
            return (i == item);
        });

        if(iter == items_.end())
        {
            items_.push_back(item);
        }
    }

    void Erase(iterator iter)
    {
		items_.erase(iter);
    }

    void Update(iterator iter, const T& item)
    {
		uint16_t index = std::distance<const_iterator>(items_.begin(), iter);
        items_[index] = item;
        items_changed_.push_back(index);
    }

	void Update(uint16_t index, const T& item)
	{
		items_[index] = item;
		items_changed_.push_back(index);
	}

    void Clear()
    {
        clear_ = true;
    }

    void Reinstall()
    {
        reinstall_ = true;
    }

    iterator Find(T& item)
    {
        auto iter = std::find_if(items_.begin(), items_.end(), [=](const T& x)->bool {
            return (item == x);
        });

        return iter;
    }

	/**
     *
     */
    T At(uint16_t index)
    {
        return items_.at(index);
    }

    /**
     *
     */
    T operator[](uint16_t index)
    {
        return At(index);
    }

    /**
     * Clears all addition, subtractions and changes to the NetworkMap. This should
     * be used if an error is encountered and the deltas need to be cleared so they don't
     * get left behind for the next update.
     */
    void ClearDeltas(void)
    {
        items_added_.clear();
        items_removed_.clear();
        items_changed_.clear();
        clear_ = false;
        reinstall_ = false;
    }

    uint16_t Capacity() { return items_.capacity(); }
    uint16_t Size() { return items_.size(); }
    iterator begin() { return items_.begin(); }
    iterator end() { return items_.end(); } 

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
        message.data.write<uint32_t>(items_.size());
        message.data.write<uint32_t>(0);
        for (auto& item : items_)
		{
            item.Serialize(message);
        }
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
		{
			message.data.write<uint32_t>(items_added_.size() + items_removed_.size() + items_changed_.size() + clear_ + reinstall_);
			message.data.write<uint32_t>(++update_counter_);

			// Remove Items
			for(auto& index : items_removed_)
			{
				message.data.write<uint8_t>(0);
				message.data.write<uint16_t>(index);
			}

			// Added Items
			for(auto& index : items_added_)
			{
				message.data.write<uint8_t>(1);
				message.data.write<uint16_t>(index);
				items_[index].Serialize(message);
			}

			// Changed Items
			for(auto& index : items_changed_)
			{
				message.data.write<uint8_t>(2);
				message.data.write<uint16_t>(index);
				items_[index].Serialize(message);
			}

			// Reinstall
			if(reinstall_)
			{
				message.data.write<uint8_t>(3);
				message.data.write<uint16_t>(items_.size());
				for(auto& item : items_)
				{
					item.Serialize(message);
				}
			}

			// Clear
			if(clear_)
			{
				message.data.write<uint8_t>(4);
				items_.clear();
			}
		}
        ClearDeltas();
    }	
private:
    std::vector<T> items_;
    std::list<uint16_t> items_added_;
    std::list<uint16_t> items_removed_;
    std::list<uint16_t> items_changed_;
    bool clear_;
    bool reinstall_;
    uint32_t update_counter_;
};

}}} // swganh::messages::containers
