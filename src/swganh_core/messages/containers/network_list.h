// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <list>
#include <boost/noncopyable.hpp>

#include "swganh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace containers {

/**
 * Expandable: Yes
 * Random Access: No
 * Mutable: No
 */
template <typename T>
class NetworkList
{
public:
    typedef typename std::list<T>::const_iterator const_iterator;
    typedef typename std::list<T>::iterator iterator;
    
    NetworkList()
        :  update_counter_(0)
        , clear_(false)
    {}

	NetworkList(std::list<T> orig)
		: items_(orig.begin(), orig.end())		
		, update_counter_(0)
                , clear_(false)
	{
	}

	NetworkList(std::vector<T> orig)
		: items_(orig.begin(), orig.end())
		, update_counter_(0)
                , clear_(false)
	{
	}

    ~NetworkList()
    {}

	std::list<T> Get() const
	{
		return items_;
	}

    /**
     * Inserts the item and queues a change to the added_items_ list.
     */
    void Add(T item)
    {
        auto iter = std::find_if(items_.begin(), items_.end(), [&item](T& x)->bool {
            return (x == item);
        });

        if(iter == items_.end())
        {
            items_.push_back(item);
            added_items_.push_back(item);
        }
    }

    /**
     * Removes an item and queues a change to the removed_items_ list.
     */
    void Remove(iterator iter)
    {
        if(iter != items_.end())
        {
            removed_items_.push_back(*iter);
            items_.erase(iter);
        }
    }

    /**
     * Inserts an item without queueing a change.
     */
    void Insert(T item)
    {
        auto iter = std::find_if(items_.begin(), items_.end(), [=](T& x)->bool {
            return (x == item);
        });

        if(iter == items_.end())
        {
            items_.push_back(item);
        }
    }

    void Clear(void)
    {
        items_.clear();
        clear_ = true;
    }

    bool Contains(const T& item)
    {
        auto iter = std::find_if(items_.begin(), items_.end(), [&item](T& x)->bool {
            return (x == item);
        });

        if(iter != items_.end())
            return true;
        else
            return false;
    }

    /**
     * Clears all addition, subtractions and changes to the NetworkMap. This should
     * be used if an error is encountered and the deltas need to be cleared so they don't
     * get left behind for the next update.
     */
    void ClearDeltas(void)
    {
        added_items_.clear();
        removed_items_.clear();
        clear_ = false;
    }

    iterator begin(void) { return items_.begin(); }
    iterator end(void) { return items_.end(); }

    uint16_t Size(void) const { return items_.size(); }

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
        for(auto& item : items_)
		{
            item.Serialize(message);
        }
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
		{
			message.data.write<uint32_t>(added_items_.size() + removed_items_.size());
			message.data.write<uint32_t>(++update_counter_);

			// Removed Items
			for (auto& x : removed_items_)
			{
				message.data.write<uint8_t>(0);         // Update Type: 0 (Remove)
				x.Serialize(message);
			}

			// Added Items
			for (auto& x : added_items_)
			{
				message.data.write<uint8_t>(1);              // Update Type: 1 (Add)
				x.Serialize(message);
			}

			// Clear Items
			if(clear_ == true)
			{
				message.data.write<uint8_t>(2);
			}
		}
        ClearDeltas();
    }

private:
    std::list<T> items_;
    std::list<T> added_items_;
    std::list<T> removed_items_;
    uint32_t update_counter_;
    bool clear_;
};

}}} // namespaces
