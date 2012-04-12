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

#include <gtest/gtest.h>

#include <boost/random.hpp>

#include "node.h"
#include <swganh/object/object.h>

using namespace quadtree;

///
class NodeTest : public testing::Test {
public:
	NodeTest()
		: root_node_(ROOT, Region(Point(-3000.0f, -3000.0f), Point(3000.0f, 3000.0f)), 0, 9, nullptr)
		, event_dispatcher_(io_service_)
	{}

	~NodeTest()
	{}

protected:
	virtual void SetUp() { }

	Node root_node_;
	anh::EventDispatcher event_dispatcher_;
	boost::asio::io_service io_service_;
};

///
TEST_F(NodeTest, CanInsertRemoveObject)
{
	std::shared_ptr<swganh::object::Object> obj(new swganh::object::Object());
	obj->SetEventDispatcher(&event_dispatcher_);
	obj->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));

	root_node_.InsertObject(obj);
	EXPECT_EQ(1, root_node_.GetContainedObjects().size());

	root_node_.RemoveObject(obj);
	EXPECT_EQ(0, root_node_.GetContainedObjects().size());
}

///
TEST_F(NodeTest, VerifyQuadrantSplit)
{
	std::shared_ptr<swganh::object::Object> obj1(new swganh::object::Object()), obj2(new swganh::object::Object()), obj3(new swganh::object::Object()), obj4(new swganh::object::Object());
	
	obj1->SetEventDispatcher(&event_dispatcher_);
	obj2->SetEventDispatcher(&event_dispatcher_);
	obj3->SetEventDispatcher(&event_dispatcher_);
	obj4->SetEventDispatcher(&event_dispatcher_);

	obj1->SetObjectId(1);
	obj2->SetObjectId(2);
	obj3->SetObjectId(3);
	obj4->SetObjectId(4);
	
	obj1->SetPosition(glm::vec3(-10.0f, 0.0f, 10.0f));
	obj2->SetPosition(glm::vec3(10.0f, 0.0f, 10.0f));
	obj3->SetPosition(glm::vec3(-10.0f, 0.0f, -10.0f));
	obj4->SetPosition(glm::vec3(10.0f, 0.0f, -10.0f));

	root_node_.InsertObject(obj1);
	root_node_.InsertObject(obj2);
	root_node_.InsertObject(obj3);
	root_node_.InsertObject(obj4);
	
	EXPECT_EQ(1, root_node_.GetLeafNodes()[NW_QUADRANT]->GetObjects().size());
	EXPECT_EQ(1, root_node_.GetLeafNodes()[NE_QUADRANT]->GetObjects().size());
	EXPECT_EQ(1, root_node_.GetLeafNodes()[SW_QUADRANT]->GetObjects().size());
	EXPECT_EQ(1, root_node_.GetLeafNodes()[SE_QUADRANT]->GetObjects().size());

	EXPECT_EQ(1, root_node_.GetLeafNodes()[NW_QUADRANT]->GetObjects()[0]->GetObjectId());
	EXPECT_EQ(2, root_node_.GetLeafNodes()[NE_QUADRANT]->GetObjects()[0]->GetObjectId());
	EXPECT_EQ(3, root_node_.GetLeafNodes()[SW_QUADRANT]->GetObjects()[0]->GetObjectId());
	EXPECT_EQ(4, root_node_.GetLeafNodes()[SE_QUADRANT]->GetObjects()[0]->GetObjectId());

	root_node_.RemoveObject(obj1);
	root_node_.RemoveObject(obj2);
	root_node_.RemoveObject(obj3);
	root_node_.RemoveObject(obj4);

	// TODO: Check that all LeafNodes have been disposed of.
}

///
TEST_F(NodeTest, CanQuery)
{
	std::shared_ptr<swganh::object::Object> obj(new swganh::object::Object());
	obj->SetEventDispatcher(&event_dispatcher_);
	obj->SetPosition(glm::vec3(10.0f, 0.0f, 10.0f));

	root_node_.InsertObject(obj);

	EXPECT_EQ(1, root_node_.Query(QueryBox( Point(0.0f, 0.0f), Point(15.0f, 15.0f) )).size());

	root_node_.RemoveObject(obj);

	EXPECT_EQ(0, root_node_.Query(QueryBox( Point(0.0f, 0.0f), Point(15.0f, 15.0f) )).size());
}

///
TEST_F(NodeTest, CanUpdateObject)
{
	std::shared_ptr<swganh::object::Object> obj(new swganh::object::Object());
	obj->SetEventDispatcher(&event_dispatcher_);
	obj->SetPosition(glm::vec3(10.0f, 0.0f, 10.0f));

	root_node_.InsertObject(obj);

	EXPECT_EQ(1, root_node_.GetLeafNodes()[NE_QUADRANT]->GetObjects().size());
	EXPECT_EQ(0, root_node_.GetLeafNodes()[SE_QUADRANT]->GetObjects().size());

	glm::vec3 new_position(10.0f, 0.0f, -10.0f);
	root_node_.UpdateObject(obj, obj->GetPosition(), new_position);
	obj->SetPosition(new_position);

	EXPECT_EQ(0, root_node_.GetLeafNodes()[NE_QUADRANT]->GetObjects().size());
	EXPECT_EQ(1, root_node_.GetLeafNodes()[SE_QUADRANT]->GetObjects().size());

	root_node_.RemoveObject(obj);
}

///
TEST_F(NodeTest, CanInsertRemoveQueryOneThousand)
{
	std::vector<std::shared_ptr<swganh::object::Object>> objects;
	boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<> random_generator(-3000.0f, 3000.0f);

	for(int i = 0; i < 1000; i++)
	{
		objects.push_back(std::make_shared<swganh::object::Object>());
		objects[i]->SetEventDispatcher(&event_dispatcher_);
		objects[i]->SetPosition(glm::vec3(random_generator(gen), 0.0f, random_generator(gen)));
		root_node_.InsertObject(objects[i]);
	}

	EXPECT_EQ(1000, root_node_.GetContainedObjects().size());
	EXPECT_EQ(1000, root_node_.Query(QueryBox(Point(-3000, -3000), Point(3000, 3000))).size());

	for(int i = 0; i < 1000; i++)
	{
		root_node_.RemoveObject(objects[i]);
	}

	EXPECT_EQ(0, root_node_.GetContainedObjects().size());
	EXPECT_EQ(0, root_node_.Query(QueryBox(Point(-3000, -3000), Point(3000, 3000))).size());
}

///
TEST_F(NodeTest, CanInsertRemoveQueryTenThousand)
{
	std::vector<std::shared_ptr<swganh::object::Object>> objects;
	boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<> random_generator(-3000.0f, 3000.0f);

	for(int i = 0; i < 10000; i++)
	{
		objects.push_back(std::make_shared<swganh::object::Object>());
		objects[i]->SetEventDispatcher(&event_dispatcher_);
		objects[i]->SetPosition(glm::vec3(random_generator(gen), 0.0f, random_generator(gen)));
		root_node_.InsertObject(objects[i]);
	}

	EXPECT_EQ(10000, root_node_.GetContainedObjects().size());
	EXPECT_EQ(10000, root_node_.Query(QueryBox(Point(-3000, -3000), Point(3000, 3000))).size());

	for(int i = 0; i < 10000; i++)
	{
		root_node_.RemoveObject(objects[i]);
	}

	EXPECT_EQ(0, root_node_.GetContainedObjects().size());
	EXPECT_EQ(0, root_node_.Query(QueryBox(Point(-3000, -3000), Point(3000, 3000))).size());
}

/*****************************************************************************/
// Implementation for the test fixture //