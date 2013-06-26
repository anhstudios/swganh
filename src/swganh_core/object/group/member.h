// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

struct Member
{
    Member(uint64_t object_id_, std::string name_)
        : object_id(object_id_)
        , name(name_)
    {}

    ~Member()
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint64_t>(object_id);
        message.data.write<std::string>(name);
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<uint64_t>(object_id);
        message.data.write<std::string>(name);
    }

    bool operator==(const Member& other)
    {
        return object_id == other.object_id;
    }

    uint64_t object_id;
    std::string name;
};

}
}