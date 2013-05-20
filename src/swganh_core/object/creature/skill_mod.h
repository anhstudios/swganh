// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

class SkillModNotFound : public std::runtime_error 
{
public:
	SkillModNotFound()
	: runtime_error("Skill mod not found")
	{}
};

struct SkillMod
{
    SkillMod()
    {}

    SkillMod(std::string identifier_, uint32_t base_, uint32_t modifier_)
        : identifier(identifier_)
        , base(base_)
        , modifier(modifier_)
    {}

    ~SkillMod(void)
    {}

    void Serialize(swganh::messages::BaselinesMessage& message)
    {
        message.data.write<uint8_t>(0);                 // Unused
        message.data.write<std::string>(identifier);    // Identifier
        message.data.write<uint32_t>(base);             // Base
        message.data.write<uint32_t>(modifier);         // Modifier
    }

    void Serialize(swganh::messages::DeltasMessage& message)
    {
        message.data.write<std::string>(identifier);    // Identifier
        message.data.write<uint32_t>(base);             // Base
        message.data.write<uint32_t>(modifier);         // Modifier
    }

    bool operator==(const SkillMod& other)
    {
        return (identifier == other.identifier);
    }

    std::string identifier;
    uint32_t base;
    uint32_t modifier;
};

}
}