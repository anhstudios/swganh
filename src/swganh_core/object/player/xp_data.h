// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct XpData
{
    XpData()
        : type("")
        , value(0){}
    XpData(std::string in_type, uint32_t in_value)
    : type(in_type)
    , value(in_value){}

    std::string type;
    uint32_t value;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint8_t>(0); // byte
        message.data.write(type);
        message.data.write(value);           
    }
    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write(type);
        message.data.write(value);
    }
    bool operator==(const XpData& other)
    {
        return (type == other.type && value == other.value);
    }
};

}
}