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

    static void SerializeBaseline(swganh::ByteBuffer& data, const QuestJournalData& t)
    {
        data.write<uint8_t>(0); // byte
        data.write<uint32_t>(t.quest_crc);
        data.write<uint64_t>(t.owner_id);
        data.write<uint16_t>(t.active_step_bitmask);
        data.write<uint16_t>(t.completed_step_bitmask);
		data.write<uint8_t>((t.completed_flag) ? 1 : 0);
        data.write<uint32_t>(t.counter);
    }

    static void SerializeDelta(swganh::ByteBuffer& data, const QuestJournalData& t)
    {
        data.write<uint32_t>(t.quest_crc);
        data.write<uint64_t>(t.owner_id);
        data.write<uint16_t>(t.active_step_bitmask);
        data.write<uint16_t>(t.completed_step_bitmask);
		data.write<uint8_t>((t.completed_flag) ? 1 : 0);
        data.write<uint32_t>(t.counter);
    }

    bool operator==(const QuestJournalData& other)
    {
        return (quest_crc == other.quest_crc);
    }
};


}
}