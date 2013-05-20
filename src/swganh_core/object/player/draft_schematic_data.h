// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct DraftSchematicData
{
    DraftSchematicData()
        : schematic_id(0)
        , schematic_crc(0)
    {}

    DraftSchematicData(uint32_t schematic_id_)
        : schematic_id(schematic_id_)
        , schematic_crc(0)
    {}

    DraftSchematicData(uint32_t schematic_id_, uint32_t schematic_crc_)
        : schematic_id(schematic_id_)
        , schematic_crc(schematic_crc_)
    {}

    ~DraftSchematicData()
    {}

    uint32_t schematic_id;
    uint32_t schematic_crc;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint32_t>(schematic_id);
        message.data.write<uint32_t>(schematic_crc);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint32_t>(schematic_id);
        message.data.write<uint32_t>(schematic_crc);
    }

    bool operator==(const DraftSchematicData& other) const
    {
        return schematic_id == other.schematic_id;
    }
};

}
}