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

#ifndef SWGANH_MESSAGES_CONTAINERS_NETWORK_SORTED_VECTOR_H_
#define SWGANH_MESSAGES_CONTAINERS_NETWORK_SORTED_VECTOR_H_

#include <vector>

#include "swganh/messages/baselines_message.h"
#include "swganh/messages/deltas_message.h"

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
        : items_(std::vector<T>())
        , items_added_(std::list<uint16_t>())
        , items_removed_(std::list<uint16_t>())
        , items_changed_(std::list<uint16_t>())
        , clear_(false)
        , reinstall_(false)
        , update_counter_(0)
    {
        items_.reserve(capacity);
    }

    ~NetworkSortedVector(void)
    {}

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

    void Update(iterator iter, T& item)
    {
        uint16_t index = std::distance<const_iterator>(items_.begin(), iter);
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

	void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(items_.size());
        message.data.write<uint32_t>(0);
        std::for_each(items_.begin(), items_.end(), [=, &message](T& item){
            item.Serialize(message);
        });
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(items_added_.size() + items_removed_.size() + items_changed_.size() + clear_ + reinstall_);
        message.data.write<uint32_t>(++update_counter_);

        // Remove Items
        std::for_each(items_removed_.begin(), items_removed_.end(), [=, &message](const uint16_t& index) {
            message.data.write<uint8_t>(0);
            message.data.write<uint16_t>(index);
        });

        // Added Items
        std::for_each(items_added_.begin(), items_added_.end(), [=, &message](const uint16_t& index) {
            message.data.write<uint8_t>(1);
            message.data.write<uint16_t>(index);
            items_[index].Serialize(message);
        });

        // Changed Items
        std::for_each(items_changed_.begin(), items_changed_.end(), [=, &message](const uint16_t& index) {
            message.data.write<uint8_t>(2);
            message.data.write<uint16_t>(index);
            items_[index].Serialize(message);
        });

        // Reinstall
        if(reinstall_)
        {
            message.data.write<uint8_t>(3);
            message.data.write<uint16_t>(items_.size());
            std::for_each(items_.begin(), items_.end(), [=, &message](T& item){
                item.Serialize(message);
            });            
        }

        // Clear
        if(clear_)
        {
            message.data.write<uint8_t>(4);
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

#endif // SWGANH_MESSAGES_CONTAINERS_NETWORK_SORTED_VECTOR_H_