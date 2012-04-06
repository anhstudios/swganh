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

#include <fstream>

using namespace quadtree;

///
TEST(QuadtreeTest, CanInsertNode)
{
	
	Node root_node(ROOT, Region(Point(-3000.0f, -3000.0f), Point(3000.0f, 3000.0f)), 0, 9);
	/**std::shared_ptr<swganh::object::Object> obj = std::make_shared<swganh::object::Object>();
	obj->SetObjectId(1);
	obj->SetPosition(glm::vec3(-10.0f, -10.0f, 0.0f));

	std::shared_ptr<swganh::object::Object> obj2 = std::make_shared<swganh::object::Object>();
	obj2->SetObjectId(2);
	obj2->SetPosition(glm::vec3(100.0f, 100.0f, 0.0f));

	std::shared_ptr<swganh::object::Object> obj3 = std::make_shared<swganh::object::Object>();
	obj3->SetObjectId(3);
	obj3->SetPosition(glm::vec3(101.0f, 101.0f, 0.0f));

	std::shared_ptr<swganh::object::Object> obj4 = std::make_shared<swganh::object::Object>();
	obj4->SetObjectId(4);
	obj4->SetPosition(glm::vec3(-200.0f, 100.0f, 0.0f));

	std::shared_ptr<swganh::object::Object> obj5 = std::make_shared<swganh::object::Object>();
	obj5->SetObjectId(5);
	obj5->SetPosition(glm::vec3(2999.0f, -2999.0f, 0.0f));

	std::shared_ptr<swganh::object::Object> obj6 = std::make_shared<swganh::object::Object>();
	obj6->SetObjectId(6);
	obj6->SetPosition(glm::vec3(102.0f, 102.0f, 0.0f));

	root_node.InsertObject(obj);
	root_node.InsertObject(obj2);
	root_node.InsertObject(obj3);
	root_node.InsertObject(obj4);
	root_node.InsertObject(obj5);
	root_node.InsertObject(obj6);
	*/

	boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<> random(-3000.0f, 3000.0f);
	for(int x = 0; x < 3000; x++)
	{
		std::shared_ptr<swganh::object::Object> obj = std::make_shared<swganh::object::Object>();
		obj->SetObjectId(x);
		obj->SetPosition(glm::vec3(random(gen), random(gen), random(gen)));
		root_node.InsertObject(obj);
	}

	QueryBox query_box(Point(-3000, -3000), Point(100, 100));
	std::cout << "Query result: " << root_node.Query(query_box).size() << std::endl;
}