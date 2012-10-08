// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/byte_buffer.h"

#include <list>

#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/deltas_message.h"

namespace swganh {
namespace messages {
namespace containers {

/**
 * Expandable: No*
 * Random Access: Yes
 * Mutable: Yes
 */
template<typename T>
class NetworkArray
{
public:
    NetworkArray(uint16_t size)
        : update_counter_(0)
        , items_(size)
        , items_added_(std::list<uint16_t>())
        , items_removed_(std::list<uint16_t>())
        , items_changed_(std::list<uint16_t>())
        , clear_(false)
    {
    }

    ~NetworkArray(void)
    {
    }

    /**
     * Sets the value of the index without queueing a change.
     */
    void Set(uint16_t index, T item)
    {
        if(index < 0 || index > Size())
            throw std::out_of_range("NetworkArray::Set index out of range.");
    
        items_[index] = item;
    }

    /**
     * Sets the value of the index and queues a change.
     */
    void Update(uint16_t index, T item)
    {
        if(index < 0 || index > Size())
            throw std::out_of_range("NetworkArray::Update index out of range.");
    
        items_[index] = item;
        items_changed_.push_back(index);
    }

    T At(uint16_t index)
    {
        if(index < 0 || index > Size())
            throw std::out_of_range("NetworkArray::At index out of range.");
    
        return items_[index];
    }

    void Resize(uint16_t size)
    {
        // Queue added indices.
        if(size > items_.size())
        {
            for(int x = items_.size() + 1; x < size; x++)
            {
                items_added_.push_back(x);
            }
        }

        // Queue removed indices.
        if(size < items_.size())
        {
            for(int x = items_.size(); x > size; x--)
            {
                items_removed_.push_back(x);
            }
        }

        items_.resize(size);
    }

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
    }

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
        message.data.write<uint32_t>(items_added_.size() + items_removed_.size() + items_changed_.size() + clear_);
        message.data.write<uint32_t>(++update_counter_);

        // Removed Items
        std::for_each(items_removed_.begin(), items_removed_.end(), [=, &message](uint16_t index) {
            message.data.write<uint8_t>(0);
            message.data.write<uint16_t>(index);
        });

        // Added Items
        std::for_each(items_added_.begin(), items_added_.end(), [=, &message](uint16_t index) {
            message.data.write<uint8_t>(1);
            message.data.write<uint16_t>(index);
            items_[index].Serialize(message);
        });

        // Changed Items
        std::for_each(items_changed_.begin(), items_changed_.end(), [=, &message](uint16_t index) {
            message.data.write<uint8_t>(2);
            message.data.write<uint16_t>(index);
            items_[index].Serialize(message);
        });

        ClearDeltas();
    }

    uint16_t Size(void) const { return items_.size(); }
    uint16_t Capacity(void) const { return items_.capacity(); }

private:
    uint16_t update_counter_;
    std::vector<T> items_;
    std::list<uint16_t> items_added_;
    std::list<uint16_t> items_removed_;
    std::list<uint16_t> items_changed_;
    bool clear_;
};

}}} // namespace swganh::messages::containers
