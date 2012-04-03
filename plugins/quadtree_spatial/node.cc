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

#include "node.h"

#include <swganh/object/object.h>

namespace quadtree
{

Node::Node(NodeQuadrant quadrant, Region region, uint32_t level, uint32_t max_level)
	: quadrant_(quadrant)
	, region_(region)
	, level_(level)
	, max_level_(max_level)
	, state_(LEAF)
{
}

Node::~Node(void)
{
}

void Node::InsertObject(std::shared_ptr<swganh::object::Object> obj)
{
	if(objects_.size() >= 1 && level_ < max_level_)
	{
		Split();
	}

	std::for_each(leaf_nodes_.begin(), leaf_nodes_.end(), [=, &obj](std::shared_ptr<Node> node){
		if(boost::geometry::within(node->GetRegion(), Region(Point(obj->GetPosition().x, obj->GetPosition().y),Point(obj->GetPosition().x, obj->GetPosition().y))))
		{
			node->InsertObject(obj);
		}
	});

	objects_.push_back(obj);
}

void Node::RemoveObject(std::shared_ptr<swganh::object::Object> obj)
{
}

void Node::Split()
{
	state_ = BRANCH;

	double half_width = boost::geometry::get<0>(region_.max_corner()) / 2.0f;
	double half_height = boost::geometry::get<1>(region_.max_corner()) / 2.0f;
	double max_width = boost::geometry::get<0>(region_.max_corner());
	double max_height = boost::geometry::get<1>(region_.max_corner());

	leaf_nodes_[NW_QUADRANT] = std::make_shared<Node>(NW_QUADRANT, Region(Point(0, 0), Point(half_width, half_height)), level_++, max_level_);
	leaf_nodes_[NE_QUADRANT] = std::make_shared<Node>(NE_QUADRANT, Region(Point(half_width, 0), Point(max_width, half_height)), level_++, max_level_);
	leaf_nodes_[SW_QUADRANT] = std::make_shared<Node>(SW_QUADRANT, Region(Point(0, half_height), Point(half_width, max_height)), level_++, max_level_);
	leaf_nodes_[SE_QUADRANT] = std::make_shared<Node>(NW_QUADRANT, Region(Point(half_width, half_height), Point(max_width, max_height)), level_++, max_level_);
}

} // namespace quadtree