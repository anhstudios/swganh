// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

namespace swganh
{
namespace object
{

enum Posture : uint8_t
{
	INVALID = 0xFF,
	UPRIGHT = 0,
	CROUCHED = 1,
	PRONE,
	SNEAKING,
	BLOCKING,
	CLIMBING,
	FLYING,
	LYING_DOWN,
	SITTING,
	SKILL_ANIMATING,
	DRIVING_VEHICLE,
	RIDING_CREATURE,
	KNOCKED_DOWN,
	INCAPACITATED,
	DEAD
};

}
}