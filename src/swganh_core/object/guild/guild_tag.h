// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct GuildTag
{
    GuildTag(uint32_t id_, std::string abbreviation_)
        : id(id_)
        , abbreviation(abbreviation_)
    {}

    ~GuildTag(void)
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        std::stringstream ss;
        ss << id << ":" << abbreviation;

        message.data.write<std::string>(ss.str());
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        std::stringstream ss;
        ss << id << ":" << abbreviation;

        message.data.write<std::string>(ss.str());
    }

    bool operator==(const GuildTag& other)
    {
        return id == other.id;
    }

	bool operator<(const GuildTag& other) const
	{
		return id < other.id;
	}
	
    uint32_t id;
    std::string abbreviation;
};

}
}