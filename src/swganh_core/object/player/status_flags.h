// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

enum StatusFlags
{
    CLEAR           = 0,
    LFG             = 0x00000001,
    HELPER          = 0x00000002,
    ROLEPLAYER      = 0x00000004,
    AFK             = 0x00000080,
    LD              = 0x00000100,
    FACTION_RANK    = 0x00000200,
    ANONYMOUS       = 0x80000000
};

}
}