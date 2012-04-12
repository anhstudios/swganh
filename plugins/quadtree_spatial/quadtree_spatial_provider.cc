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

#include "quadtree_spatial_provider.h"

#include "anh/logger.h"

using std::shared_ptr;

using anh::app::KernelInterface;
using swganh::object::Object;
using namespace quadtree;

QuadtreeSpatialProvider::QuadtreeSpatialProvider(anh::app::KernelInterface* kernel)
	: SpatialProviderInterface(kernel)
	, root_node_(ROOT, Region(Point(-8300.0f, -8300.0f), Point(8300.0f, 8300.0f)), 0, 9, nullptr)
{
}

QuadtreeSpatialProvider::~QuadtreeSpatialProvider(void)
{
}

void QuadtreeSpatialProvider::AddObject(shared_ptr<Object> obj)
{
	root_node_.InsertObject(obj);
}

void QuadtreeSpatialProvider::RemoveObject(shared_ptr<Object> obj)
{
	root_node_.RemoveObject(obj);
}

void QuadtreeSpatialProvider::UpdateObject(shared_ptr<Object> obj, glm::vec3 old_position, glm::vec3 new_position)
{
	root_node_.UpdateObject(obj, old_position, new_position);

}

std::vector<std::shared_ptr<swganh::object::Object>> QuadtreeSpatialProvider::GetObjectsInRange(glm::vec3 point, float range)
{
	return std::vector<shared_ptr<Object>>();
}
