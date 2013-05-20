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

	static void SerializeBaseline(swganh::ByteBuffer& data, const XpData& t)
    {
        data.write<uint8_t>(0); // byte
        data.write(t.type);
        data.write(t.value);           
    }

    static void SerializeDelta(swganh::ByteBuffer& data, const XpData& t)
    {
        data.write(t.type);
        data.write(t.value);
    }

    bool operator==(const XpData& other)
    {
        return (type == other.type && value == other.value);
    }
};

}
}