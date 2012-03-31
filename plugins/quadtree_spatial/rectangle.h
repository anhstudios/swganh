/*
 This file is part of SWGANH. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2012 The SWG:ANH Team

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace quadtree
{

class Rectangle
{
	Rectangle(float x, float y, uint32_t width, uint32_t height);

	bool Intersects(const Rectangle& other);

private:
	float x;
	float y;
	uint32_t width;
	uint32_t height;
};

}

#endif // RECTANGLE_H_