/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2011 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef SWGANH_NETWORK_NETWORK_LIST_H_
#define SWGANH_NETWORK_NETWORK_LIST_H_

#include <list>

#include "anh/byte_buffer.h"

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
        : update_counter_(0)
        , items_(std::list<T>())
        , added_items_(std::list<T>())
        , removed_items_(std::list<T>())
        , clear_(false)
    {}

    ~NetworkList()
    {}

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

    iterator Begin(void) { return items_.begin(); }
    iterator End(void) { return items_.end(); }

    uint16_t Size(void) const { return items_.size(); }

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(items_.size());
        message.data.write<uint32_t>(0);
        std::for_each(items_.begin(), items_.end(), [=, &message](T item) {
            item.Serialize(message);
        });
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(added_items_.size() + removed_items_.size());
        message.data.write<uint32_t>(++update_counter_);

        // Removed Items
        std::for_each(removed_items_.begin(), removed_items_.end(), [=, &message](T x) {
            message.data.write<uint8_t>(0);         // Update Type: 0 (Remove)
            x.Serialize(message);
        });

        // Added Items
        std::for_each(added_items_.begin(), added_items_.end(), [=, &message](T x) {
            message.data.write<uint8_t>(1);              // Update Type: 1 (Add)
            x.Serialize(message);
        });

        // Clear Items
        if(clear_ == true)
        {
            message.data.write<uint8_t>(2);
        }

        ClearDeltas();
    }

private:
    uint32_t update_counter_;
    std::list<T> items_;
    std::list<T> added_items_;
    std::list<T> removed_items_;
    bool clear_;
};

}}} // namespaces

#endif // SWGANH_NETWORK_NETWORK_VECTOR_H_