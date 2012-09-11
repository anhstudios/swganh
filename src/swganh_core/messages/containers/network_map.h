// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <map>
#include <list>

#include "swganh/byte_buffer.h"

namespace swganh {
namespace messages {
namespace containers {

/**
 * Expandable: Yes
 * Random Access: Yes
 * Mutable: Yes
 */
template <typename I, typename T>
class NetworkMap
{
public:
    typedef typename std::map<I, T>::const_iterator const_iterator;
    typedef typename std::map<I, T>::iterator iterator;
    
    NetworkMap()
        : update_counter_(0)
        , items_(std::map<I, T>())
        , items_added_(std::list<I>())
        , items_removed_(std::list<T>())
        , items_changed_(std::list<I>())
        , clear_(false)
        , reinstall_(false)
    {}

    ~NetworkMap()
    {}

    /**
     * Inserts a new entry into the NetworkMap without
     * queueing a update.
     */
    void Insert(const I& index, T item)
    {
        auto iter = items_.find(index);
        if(iter == items_.end())
        {
            items_.insert(std::pair<I, T>(index, item));
        }
    }

    /**
     * Erases an entry from the NetworkMap without
     * queueing a update.
     */
    void Erase(const I& index)
    {
        auto iter = items_.find(index);
        if(iter != items_.end())
        {
            items_.erase(iter);
        }
    }

    /**
     * Inserts a new entry into the NetworkMap and
     * queues a update.
     */
    void Add(const I& index, T item)
    {
        auto iter = items_.find(index);
        if(iter == items_.end())
        {
            items_.insert(std::pair<I, T>(index, item));
            items_added_.push_back(index);
        }
    }

    /**
     * Erases an entry from the NetworkMap and
     * queues a update.
     */
    void Remove(iterator iter)
    {
        if(iter != items_.end())
        {
            items_removed_.push_back(iter->second);
            items_.erase(iter);
        }
    }

    /**
     * Updates an entry in the NetworkMap and
     * queues a update.
     */
    void Update(const I& index, T item)
    {
        auto iter = items_.find(index);
        if(iter != items_.end())
        {
            iter->second = item;
            items_changed_.push_back(index);
        }
    }

    /**
     * Searches for the index in the NetworkMap.
     */
    bool Contains(const I& index)
    {
        auto iter = items_.find(index);
        if(iter != items_.end())
            return true;
        else
            return false;
    }

    iterator Find(const I& index)
    {
        return items_.find(index);
    }

    /**
     * Clears the NetworkMap.
     */
    void Clear(void)
    {
        items_.clear();
        clear_ = true;
    }

    /**
     * Causes the NetworkMap to reinstall on next Delta serialize.
     */
    void Reinstall(void)
    {
        reinstall_ = true;
    }

    /**
     * Sets a new contents and causes the NetworkMap to reinstall on next Deltas serialize.
     */
    void Reinstall(const std::map<I, T>& new_items)
    {
        items_ = new_items;
        reinstall_ = true;
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

    iterator begin() { return items_.begin(); }
    iterator end() { return items_.end(); }

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(items_.size());
        message.data.write<uint32_t>(0);
        std::for_each(items_.begin(), items_.end(), [=, &message](std::pair<I, T> item) {
            item.second.Serialize(message);
        });
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        uint32_t size = items_added_.size() + items_removed_.size() + items_changed_.size() + reinstall_ + clear_;
        message.data.write<uint32_t>(size);
        message.data.write<uint32_t>(++update_counter_);

        // Added Items
        std::for_each(items_added_.begin(), items_added_.end(), [=, &message](I index){
            message.data.write<uint8_t>(0);
            items_[index].Serialize(message);
        });

        // Removed Items
        std::for_each(items_removed_.begin(), items_removed_.end(), [=, &message](T item){
            message.data.write<uint8_t>(1);
            item.Serialize(message);
        });

        // Changed Items
        std::for_each(items_changed_.begin(), items_changed_.end(), [=, &message](I index){
            message.data.write<uint8_t>(2);
            items_[index].Serialize(message);
        });

        if(reinstall_)
        {
            message.data.write<uint8_t>(3);
            message.data.write<uint16_t>(items_.size());
            std::for_each(items_.begin(), items_.end(), [=, &message](std::pair<I, T> item) {
                item.second.Serialize(message);
            });
        }

        if(clear_)
        {
            message.data.write<uint8_t>(4);
        }

        ClearDeltas();
    }

private:
    uint32_t update_counter_;
    std::map<I, T> items_;
    std::list<I> items_added_;
    std::list<T> items_removed_;
    std::list<I> items_changed_;
    bool clear_;
    bool reinstall_;
};

}}} // swganh::messages::containers
