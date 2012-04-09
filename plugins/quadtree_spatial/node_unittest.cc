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
	
	Node root_node(ROOT, Region(Point(-3000.0f, -3000.0f), Point(3000.0f, 3000.0f)), 0, 9, nullptr);
	std::vector<std::shared_ptr<swganh::object::Object>> objects;
	
	boost::random::mt19937 gen;
	boost::random::uniform_real_distribution<> random(-3000.0f, 3000.0f);
	for(int x = 0; x < 3000; x++)
	{
		std::shared_ptr<swganh::object::Object> obj = std::make_shared<swganh::object::Object>();
		obj->SetObjectId(x);
		obj->SetPosition(glm::vec3(random(gen), random(gen), random(gen)));
		objects.push_back(obj);
	}

	for(auto i = objects.begin(); i != objects.end(); i++)
	{
		root_node.InsertObject((*i));
	}

	QueryBox query_box(Point(-3000, -3000), Point(100, 100));
	std::cout << "Query result: " << root_node.Query(query_box).size() << std::endl;

	QueryBox query_box2(Point(-3000, -3000), Point(3000, 3000));
	std::cout << "Query 2 result: " << root_node.Query(query_box2).size() << std::endl;

	boost::random::uniform_int_distribution<> rand2(1, 3000);
	auto object_selected = objects[rand2(gen)];
	root_node.UpdateObject(object_selected, object_selected->GetPosition(), glm::vec3(10.0f, 10.0f, 10.0f));

	for(auto i = objects.begin(); i != objects.end(); i++)
	{
		root_node.RemoveObject((*i));
	}

	QueryBox query_box3(Point(-3000, -3000), Point(100, 100));
	std::cout << "Query 3 result: " << root_node.Query(query_box3).size() << std::endl;

	QueryBox query_box4(Point(-3000, -3000), Point(3000, 3000));
	std::cout << "Query 4 result: " << root_node.Query(query_box4).size() << std::endl;
}