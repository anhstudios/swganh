// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "node.h"

#include "swganh_core/object/object.h"

namespace quadtree
{

Node::Node(NodeQuadrant quadrant, Region region, uint32_t level, uint32_t max_level, Node* parent)
	: quadrant_(quadrant)
	, region_(region)
	, level_(level)
	, max_level_(max_level)
	, state_(LEAF)
	, parent_(parent)
{
	// If this is the root node, we need to do an initial split.
	if(quadrant_ == ROOT)
		Split();
}

Node::~Node(void)
{
}

void Node::InsertObject(std::shared_ptr<swganh::object::Object> obj)
{
	// If the amount of objects contained is equal to or exceeds (in the case of objects not fitting
	// completely into one node), and we havn't reached the "maximum level" count, and we are a LEAF
	// node, Split().
	if(objects_.size() >= 1 && level_ < max_level_ && state_ == LEAF)
	{
		Split();
	}

	// Flipped to true if a child node of proper side was found to
	// contain the object. Otherwise, the object will be added to 
	// this node.
	bool success = false;

	if(state_ == BRANCH)
	{
		std::for_each(leaf_nodes_.begin(), leaf_nodes_.end(), [=, &obj, &success](std::shared_ptr<Node> node){
			// If we can fit within the node, traverse.
			if(boost::geometry::within( Point(obj->GetPosition().x, obj->GetPosition().z) , node->GetRegion()))
			{
				node->InsertObject(obj);
				success = true;
				return;
			}
		});
	}

	if(success)
		return;

	objects_.insert(obj);
}

void Node::RemoveObject(std::shared_ptr<swganh::object::Object> obj)
{
	// Search this node for the object by id, if it it found
	// we can return;
	for(auto i = objects_.begin(); i != objects_.end(); )
	{
		if(obj->GetObjectId() == (*i)->GetObjectId())
		{
			i = objects_.erase(i);
			return;
		}
		i++;
	}

	// We didn't find the object in this branch, traverse through
	// each leaf node if we are a BRANCH.
	if(state_ == BRANCH)
	{
		Point obj_point(obj->GetPosition().x, obj->GetPosition().z);
		for(std::shared_ptr<Node> node : leaf_nodes_)
		{
			// If we can actually fit inside the node, traverse farther.
			if(boost::geometry::within(obj_point, node->GetRegion()))
			{
				node->RemoveObject(obj);
				return;
			}
		}
	}

}

void Node::Split()
{
	if(state_ == BRANCH)
		return;

	state_ = BRANCH;

	Point center((region_.min_corner().x() + region_.max_corner().x()) / 2, (region_.min_corner().y() + region_.max_corner().y()) / 2);
	Point upper_center((region_.min_corner().x() + region_.max_corner().x()) / 2, region_.max_corner().y());
	Point left_center(region_.min_corner().x(), (region_.min_corner().y() + region_.max_corner().y()) / 2);
	Point right_center(region_.max_corner().x(), (region_.min_corner().y() + region_.max_corner().y()) / 2);
	Point bottom_center((region_.min_corner().x() + region_.max_corner().x()) / 2, region_.min_corner().y());

	leaf_nodes_[NW_QUADRANT] = std::make_shared<Node>(NW_QUADRANT, Region(left_center, upper_center), level_ + 1, max_level_, this);
	leaf_nodes_[NE_QUADRANT] = std::make_shared<Node>(NE_QUADRANT, Region(center, region_.max_corner()), level_ + 1, max_level_, this);
	leaf_nodes_[SW_QUADRANT] = std::make_shared<Node>(SW_QUADRANT, Region(region_.min_corner(), center), level_ + 1, max_level_, this);
	leaf_nodes_[SE_QUADRANT] = std::make_shared<Node>(SE_QUADRANT, Region(bottom_center, right_center), level_ + 1, max_level_, this);

	for(auto i = objects_.begin(); i != objects_.end();)
	{
		auto obj = (*i);
		bool success = false;
		for(std::shared_ptr<Node> node : leaf_nodes_)
		{
			if(boost::geometry::within(Point(obj->GetPosition().x, obj->GetPosition().z) , node->GetRegion()))
			{
				i = objects_.erase(i);
				node->InsertObject(std::move(obj));
				success = true;
				break;
			}
		}

		if(!success)
			i++;
	}
}

std::list<std::shared_ptr<swganh::object::Object>> Node::Query(QueryBox query_box)
{
	std::list<std::shared_ptr<swganh::object::Object>> return_list;

	std::for_each(objects_.begin(), objects_.end(), [=,& return_list](std::shared_ptr<swganh::object::Object> obj) {
		if(boost::geometry::within(Point(obj->GetPosition().x, obj->GetPosition().z), query_box))
			return_list.push_back(obj);
	});

	if(state_ == BRANCH)
	{
		for(std::shared_ptr<Node> node : leaf_nodes_)
		{
			// Node is within Query Box.
			if(boost::geometry::within(node->GetRegion(), query_box))
			{
				return_list.splice(return_list.end(), node->GetContainedObjects());
				continue;
			}
			
			// Query Box is within node.
			if(boost::geometry::within(query_box, node->GetRegion()))
			{
				return_list.splice( return_list.end(), node->Query(query_box) );
				break;
			}

			// Query Box intersects with node.
			if(boost::geometry::intersects(query_box, node->GetRegion()))
			{
				return_list.splice( return_list.end(), node->Query(query_box) );
			}
		}
	}

	return return_list;
}

std::list<std::shared_ptr<swganh::object::Object>> Node::GetContainedObjects(void)
{
	std::list<std::shared_ptr<swganh::object::Object>> objs(objects_.begin(), objects_.end());
	if(state_ == BRANCH)
	{
		for(const std::shared_ptr<Node> node : leaf_nodes_)
		{
			objs.splice(objs.end(), node->GetContainedObjects());
		}
	}
	return objs;
}

void Node::UpdateObject(std::shared_ptr<swganh::object::Object> obj, const glm::vec3& old_position, const glm::vec3& new_position)
{
	Point old_position_point(old_position.x, old_position.z);
	Point new_position_point(new_position.x, new_position.z);

	// Check the objects of this node.
	for(auto i = objects_.begin(); i != objects_.end(); i++) {
		auto node_obj = (*i);
		if(node_obj->GetObjectId() == obj->GetObjectId())
		{
			// If we are in the same node, we don't need to do anything.
			if(boost::geometry::within(new_position_point, region_))
			{
				return;
			}

			// Move our object from this node to a new node.
			std::shared_ptr<Node> node = GetRootNode_()->GetNodeWithinPoint_(new_position_point);
			objects_.erase(i);
			node->InsertObject(obj);
			return;
		}
	};

	if(state_ == BRANCH)
	{
		for(std::shared_ptr<Node> node : leaf_nodes_)
		{
			// Go further into the tree if our point is within our child node.
			if(boost::geometry::within(old_position_point, node->GetRegion()))
			{
				node->UpdateObject(obj, old_position, new_position);
				return;
			}
		}
	}
}

std::shared_ptr<Node> Node::GetNodeWithinPoint_(Point point)
{
	// If we don't within the actual Spatial Indexing area, bail.
	if(!boost::geometry::within(point, region_))
		throw new std::runtime_error("Quadtree: Object position out of bounds.");

	if(state_ == BRANCH)
	{
		// See if we can fit inside leaf_nodes_
		for(std::shared_ptr<Node> node : leaf_nodes_)
		{
			if(boost::geometry::within(point, node->GetRegion()))
			{
				return node->GetNodeWithinPoint_(point);
			}
		}
	}

	// If not, we are between or in this node.
	return std::shared_ptr<Node>(shared_from_this());
}

} // namespace quadtree