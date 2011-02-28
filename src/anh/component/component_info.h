/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBANH_COMPONENT_COMPONENT_INFO
#define LIBANH_COMPONENT_COMPONENT_INFO

#include <anh/hash_string.h>

namespace anh {
namespace component {

typedef anh::HashString ComponentType;

/**
 * \brief Holds type and options information about a specific component type.
 *
 * Options and Variables:
 * type - A Hashed string that specifies the type of component.
 * update_every_frame - If flagged true, the components update function will be called every frame.
 */
struct ComponentInfo
{
	ComponentInfo(ComponentType _type, bool _update_every_frame = false)
		: type(_type)
		, update_every_frame(_update_every_frame) { }

	ComponentType type;
	bool update_every_frame;
};

}
}

#endif // LIBANH_COMPONENT_COMPONENT_INFO