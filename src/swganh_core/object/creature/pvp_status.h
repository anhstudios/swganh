// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

enum PvpStatus
{
    PvPStatus_None          = 0x00000000,
    PvPStatus_Attackable    = 0x00000001,
    PvPStatus_Aggressive    = 0x00000002,
    PvPStatus_Overt         = 0x00000004,
    PvPStatus_Tef           = 0x00000008,
    PvPStatus_Player        = 0x00000010,
    PvPStatus_Enemy         = 0x00000020,
    PvPStatus_Duel          = 0x00000040
};

}
}