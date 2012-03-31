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

#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <memory>

#include <boost/array.hpp>

namespace swganh {
namespace object {
	class Object;
}} // namespace swganh::object

namespace quadtree
{

enum NodeQuadrant
{
	NW_QUADRANT,
	NE_QUADRANT,
	SW_QUADRANT,
	SE_QUADRANT,
	ROOT
}; // enum LeafNodeQuadrant

/**
 *
 */
class Node
{
public:
	enum NodeState
	{
		BRANCH,
		LEAF
	};

	Node(NodeQuadrant quadrant, std::vector<std::shared_ptr<swganh::object::Object>> objects, uint32_t level);
	~Node(void);

	void Split();

	const NodeQuadrant& GetQuadrant(void) { return quadrant_; }
	const uint32_t& GetLevel(void) { return level_; }
	const NodeState& GetState(void) { return state_; }
	const std::vector<std::shared_ptr<swganh::object::Object>>& GetObjects(void) { return objects_; }

private:
	uint32_t level_;
	NodeQuadrant quadrant_;
	NodeState state_;
	std::vector<std::shared_ptr<swganh::object::Object>> objects_;
	boost::array<std::shared_ptr<Node>, 4> leaf_nodes_;
};

} // namespace quadtree

#endif // NODE_H_