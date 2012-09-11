// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <sstream>

#include <boost/thread/mutex.hpp>

#include "swganh_core/object/object.h"

#include "swganh_core/messages/containers/network_list.h"

namespace swganh {
namespace object {


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

    uint32_t id;
    std::string abbreviation;
};

class GuildFactory;
class GuildMessageBuilder;
class Guild : public swganh::object::Object {
public:
    Guild();
    ~Guild();

    void AddGuildTag(uint32_t guild_id, std::string abbreviation);
    void RemoveGuildTag(uint32_t guild_id);
    swganh::messages::containers::NetworkList<GuildTag> GetGuildList();

    virtual uint32_t GetType() const { return type; }
    const static uint32_t type = 0x444C4947;

private:
	typedef swganh::ValueEvent<std::shared_ptr<Guild>> GuildEvent;
    swganh::messages::containers::NetworkList<GuildTag>     guild_list_;
};

}}  // namespaces swganh::object
