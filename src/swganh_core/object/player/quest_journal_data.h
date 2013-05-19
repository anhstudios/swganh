// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct QuestJournalData
{
    QuestJournalData()
        : quest_crc(0)
        , owner_id(0)
        , active_step_bitmask(0)
        , completed_step_bitmask(0)
        , completed_flag(false)
        , counter(0){}

    uint32_t quest_crc;
    uint64_t owner_id;
    uint16_t active_step_bitmask;
    uint16_t completed_step_bitmask;
    bool completed_flag;
    uint32_t counter;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint8_t>(0); // byte
        message.data.write<uint32_t>(quest_crc);
        message.data.write<uint64_t>(owner_id);
        message.data.write<uint16_t>(active_step_bitmask);
        message.data.write<uint16_t>(completed_step_bitmask);
        if (completed_flag)
            message.data.write<uint8_t>(1);
        else
            message.data.write<uint8_t>(0);
        message.data.write<uint32_t>(counter);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(quest_crc);
        message.data.write<uint64_t>(owner_id);
        message.data.write<uint16_t>(active_step_bitmask);
        message.data.write<uint16_t>(completed_step_bitmask);
        if (completed_flag)
            message.data.write<uint8_t>(1);
        else
            message.data.write<uint8_t>(0);
        message.data.write<uint32_t>(counter);
    }

    bool operator==(const QuestJournalData& other)
    {
        return (quest_crc == other.quest_crc);
    }
};


}
}