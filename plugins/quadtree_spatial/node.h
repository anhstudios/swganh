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
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <glm/glm.hpp>

namespace swganh {
namespace object {
	class Object;
}} // namespace swganh::object

namespace quadtree
{

typedef boost::geometry::model::box< boost::geometry::model::d2::point_xy<double> > Region;
typedef boost::geometry::model::box< boost::geometry::model::d2::point_xy<double> > QueryBox;
typedef boost::geometry::model::d2::point_xy<double> Point;

enum NodeQuadrant
{
	NW_QUADRANT = 0,
	NE_QUADRANT,
	SW_QUADRANT,
	SE_QUADRANT,
	ROOT
}; // enum LeafNodeQuadrant

/**
 * \brief A Quadtree node.
 */
class Node : public std::enable_shared_from_this<Node>
{
public:
	enum NodeState
	{
		BRANCH,
		LEAF
	};

	Node(
		NodeQuadrant quadrant, 
		Region region,
		uint32_t level,
		uint32_t max_level,
		Node* parent = NULL
		);

	~Node(void);

	void InsertObject(std::shared_ptr<swganh::object::Object> obj);
	void RemoveObject(std::shared_ptr<swganh::object::Object> obj);
	void UpdateObject(std::shared_ptr<swganh::object::Object> obj, const glm::vec3& old_position, const glm::vec3& new_position);
	void Split();
	std::vector<std::shared_ptr<swganh::object::Object>> Query(QueryBox query_box);

	const NodeQuadrant& GetQuadrant(void) { return quadrant_; }
	const uint32_t& GetLevel(void) { return level_; }
	const NodeState& GetState(void) { return state_; }
	const Region& GetRegion(void) { return region_; }
	const boost::array<std::shared_ptr<Node>, 4>& GetLeafNodes(void) { return leaf_nodes_; }
	const std::vector<std::shared_ptr<swganh::object::Object>>& GetObjects(void) { return objects_; }
	const std::vector<std::shared_ptr<swganh::object::Object>> GetContainedObjects(void);

protected:
	void InsertObject_(std::shared_ptr<swganh::object::Object> obj);
	void RemoveObject_(std::shared_ptr<swganh::object::Object> obj);
	std::shared_ptr<Node> GetNodeWithinPoint_(Point point);
	
	Node* GetRootNode_(void) { 	
		// Go to the root.
		if(parent_ != NULL)
			return parent_->GetRootNode_();
		else
			return this;
	}

private:
	NodeQuadrant quadrant_;
	Region region_;
	uint32_t level_;
	uint32_t max_level_;
	NodeState state_;
	std::vector<std::shared_ptr<swganh::object::Object>> objects_;
	boost::array<std::shared_ptr<Node>, 4> leaf_nodes_;
	Node* parent_;
};

} // namespace quadtree

#endif // NODE_H_