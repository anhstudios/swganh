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

#include "quadtree.h"

using std::shared_ptr;
using swganh::object::Object;

namespace quadtree
{

Quadtree::Quadtree(uint32_t max_objects_per_node, uint32_t max_level)
	: max_objects_per_node_(max_objects_per_node)
	, max_level_(max_level)
	, root_node_(ROOT, std::vector<std::shared_ptr<swganh::object::Object>>(), 0)
{
}

Quadtree::~Quadtree(void)
{
}

void Quadtree::AddObject(shared_ptr<Object> obj)
{
}

void Quadtree::RemoveObject(shared_ptr<Object> obj)
{
}

void Quadtree::UpdateObject(shared_ptr<Object> obj)
{
}

} // namespace quadtree