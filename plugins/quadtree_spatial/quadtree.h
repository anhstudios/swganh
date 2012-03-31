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

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "node.h"
#include "rectangle.h"

namespace swganh {
namespace object {
	class Object;
}} // namespace swganh::object

namespace quadtree
{

class Quadtree
{
public:
	Quadtree(uint32_t max_object_per_node = 1, uint32_t max_level = 6);
	~Quadtree(void);

	void AddObject(std::shared_ptr<swganh::object::Object> obj);
	void RemoveObject(std::shared_ptr<swganh::object::Object> obj);
	void UpdateObject(std::shared_ptr<swganh::object::Object> obj);

	std::vector<std::shared_ptr<swganh::object::Object>> Query(glm::vec2, Rectangle);
private:
	uint32_t max_objects_per_node_;
	uint32_t max_level_;

	Node root_node_;
};

} // namespace quadtree

#endif // QUADTREE_H_