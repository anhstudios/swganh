// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh_core/messages/baselines_message.h"
#include "swganh_core/messages/deltas_message.h"

namespace swganh
{
namespace object
{

struct Skill
{
    Skill(std::string name_)
        : name(name_)
    {}

    ~Skill()
    {}

    std::string name;

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<std::string>(name);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(name);
    }

    bool operator==(const Skill& other)
    {
        return (name == other.name);
    }
};
}
}