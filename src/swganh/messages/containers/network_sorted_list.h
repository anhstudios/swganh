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

#ifndef SWGANH_MESSAGES_NETWORK_SORTED_VECTOR_H_
#define SWGANH_MESSAGES_NETWORK_SORTED_VECTOR_H_

#include <list>
#include <map>

#include "anh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace containers {

/**
 * A special container which traces changes to lists which are
 * transfered over the network.
 *
 * Types used are required to have the following functions:
 * - Serialize(anh::ByteBuffer& message)
 * - operator ==(T other)
 */
template <typename T>
class NetworkSortedList
{
public:    
    typedef typename std::map<uint16_t, T>::const_iterator const_iterator;
    typedef typename std::map<uint16_t, T>::iterator iterator;
    
    NetworkSortedList()
        : update_counter_(0)
        , items_(std::map<uint16_t, T>())
        , items_added_(std::list<uint16_t>())
        , items_removed_(std::list<uint16_t>())
        , items_changed_(std::list<uint16_t>())
        , clear_(false)
        , reinstall_(false)
    {}

    ~NetworkSortedList()
    {}

    /**
     *
     */
    uint16_t Add(T item)
    {
        auto iter = std::find_if(items_.begin(), items_.end(), [=](std::pair<uint16_t, T> x)->bool {
            return (x.second == item);
        });

        if(iter == items_.end())
        {
            uint16_t index = items_.size() + 1;
            items_.insert(std::pair<uint16_t, T>(index, item));
            items_added_.push_back(index);
            return index;
        }
        else
        {
            return iter->first;
        }
    }

    /**
     *
     */
    void Remove(typename std::map<uint16_t, T>::const_iterator iter)
    {
        if(iter != items_.end())
        {
            items_removed_.push_back(iter->first);
            items_.erase(iter);
        }
    }

    /**
     * Inserts an item into the list without queue an update.
     */
    uint16_t Insert(T item)
    {
        uint16_t index = items_.size();
        items_.insert(std::pair<uint16_t, T>(index, item));
        return index;
    }

    /**
     * Erases an item from the list by index without
     * queuing an update.
     */
    void Erase(uint16_t index)
    {
        auto iter = std::find_if(items_.begin(), items_.end(), [=](std::pair<uint16_t, T> x)->bool {
            return (x.first == index);
        });

        if(iter != items_.end())
            items_.erase(iter);
    }

    /**
     *
     */
    void Update(uint16_t index, T& item)
    {
        auto iter = items_.find(index);
        if(iter != items_.end())
        {
            iter->second = item;
            items_changed_.push_back(iter->first);
        }
        else
            throw new std::out_of_range("NetworkIndexList::Update index out of range.");
    }

    void Clear(void)
    {
        clear_ = true;
        items_.clear();
    }

    /**
     *
     */
    T At(uint16_t index)
    {
        auto iter = items_.find(index);
        if(iter != items_.end())
            return iter->second;
        else
            throw new std::out_of_range("NetworkIndexList::At index out of range.");
    }

    /**
     *
     */
    T operator[](uint16_t index)
    {
        return At(index);
    }

    /**
     *
     */
    iterator Find(const T& item)
    {
        auto iter = std::find_if(items_.begin(), items_.end(), [&item](std::pair<uint16_t, T> x)->bool {
            return (x.second == item);
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

    uint16_t Size(void) const { return items_.size(); }
    
    iterator begin() { return items_.begin(); }
    iterator end() { return items_.end(); }

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(items_.size());
        message.data.write<uint32_t>(0);
        std::for_each(items_.begin(), items_.end(), [=, &message](std::pair<uint16_t, T> item) {
            item.second.Serialize(message);
        });
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(items_added_.size() + items_removed_.size() + items_changed_.size() + clear_ + reinstall_);
        message.data.write<uint32_t>(++update_counter_);

        // Removed Items
        std::for_each(items_removed_.begin(), items_removed_.end(), [=, &message](uint16_t index) {
            message.data.write<uint8_t>(0);         // Update Type: 0 (Remove)
            message.data.write<uint16_t>(index);    // Index
        });

        // Added Items
        std::for_each(items_added_.begin(), items_added_.end(), [=, &message](uint16_t index) {
            message.data.write<uint8_t>(1);         // Update Type: 1 (Add)
            message.data.write<uint16_t>(index);
            items_[index].Serialize(message);
        });

        // Changed Items
        std::for_each(items_changed_.begin(), items_changed_.end(), [=, &message](uint16_t index) {
            message.data.write<uint8_t>(2);
            message.data.write<uint16_t>(index);
            items_[index].Serialize(message);
        });

        // Reinstall Items
        if(reinstall_)
        {
            message.data.write<uint8_t>(3);
            message.data.write<uint16_t>(items_.size());
            std::for_each(items_.begin(), items_.end(), [=, &message](std::pair<uint16_t, T> item) {
                item.second.Serialize(message);
            });
        }

        // Clear Items
        if(clear_)
        {
            message.data.write<uint8_t>(4);
        }

        ClearDeltas();
    }

private:
    uint32_t update_counter_;
    std::map<uint16_t, T> items_;
    std::list<uint16_t> items_added_;
    std::list<uint16_t> items_removed_;
    std::list<uint16_t> items_changed_;
    bool clear_;
    bool reinstall_;
};

}}} // namespaces

#endif // SWGANH_MESSAGES_NETWORK_INDEXED_VECTOR_H_