/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "pob_interpreter.h"

#include "../../iff.h"
#include "../../filenode.h"

using namespace anh::clientfile_manager::iff::interpreters;
using namespace std::tr1::placeholders;
using namespace std;
using namespace boost;

pob_interpreter::NodeNameIndex pob_interpreter::nameLookup_;

pob_interpreter::pob_interpreter()
	: interpreter_interface()
{
	if(nameLookup_.empty())
	{
		nameLookup_.insert(make_pair<string, NodeFunctor>(string("INDX"), bind(&pob_interpreter::_handleINDX, _1, _2)));
		nameLookup_.insert(make_pair<string, NodeFunctor>(string("0003DATA"), bind(&pob_interpreter::_handle0003DATA, _1, _2)));
		nameLookup_.insert(make_pair<string, NodeFunctor>(string("0005DATA"), bind(&pob_interpreter::_handle0005DATA, _1, _2)));
		nameLookup_.insert(make_pair<string, NodeFunctor>(string("PRTL"), bind(&pob_interpreter::_handlePRTL, _1, _2)));
		nameLookup_.insert(make_pair<string, NodeFunctor>(string("PRTSPRTL"), bind(&pob_interpreter::_handlePRTL, _1, _2)));
		nameLookup_.insert(make_pair<string, NodeFunctor>(string("PRTL0004"), bind(&pob_interpreter::_handlePRTL0004, _1, _2)));
		nameLookup_.insert(make_pair<string, NodeFunctor>(string("0000VERT"), bind(&pob_interpreter::_handle0000VERT, _1, _2)));
	}
}

void pob_interpreter::interpret(std::shared_ptr<anh::clientfile_manager::iff::file_node> node)
{
	NodeNameIndexIterator it = nameLookup_.find(node->name());
	if( it != nameLookup_.cend())
	{
		it->second(this, node->data());
	}
}

void pob_interpreter::_handle0003DATA(pob_interpreter* pob, anh::ByteBuffer& data)
{
	int test = data.read<std::uint32_t>();

	pob->portals_.reserve(test);
	pob->cells_.reserve(data.read<std::uint32_t>());
}

void pob_interpreter::_handlePRTL(pob_interpreter* pob, anh::ByteBuffer& data)
{
	Portal newP;

	std::uint32_t size = data.read<std::uint32_t>();
	newP.vertices.reserve(size);
	for(std::uint32_t i = 0; i < size; ++i)
	{
		glm::vec3 vector;
		vector.x = data.read<float>();
		vector.y = data.read<float>();
		vector.z = data.read<float>();
		newP.vertices.push_back(std::move(vector));
	}

	pob->portals_.push_back(newP);
}

void pob_interpreter::_handle0005DATA(pob_interpreter* pob, anh::ByteBuffer& data)
{
	Cell cell;
	cell.links.reserve(data.read<std::uint32_t>());
	cell.unkFlag2 = data.read<char>();
	cell.name = data.read<std::string>(false,true);
	cell.mesh = data.read<std::string>(false,true);
	cell.unkFlag3 = data.read<char>();
	cell.collision = data.read<std::string>(false,true);
	pob->cells_.push_back(std::move(cell));
}

void pob_interpreter::_handle0000VERT(pob_interpreter* pob, anh::ByteBuffer& data)
{
	if(!(pob->cells_.size() > 0))
		return;

	Cell& cell = pob->cells_[pob->cells_.size()-1];
	
	std::uint32_t count = data.size()/12;
	cell.vertices.reserve(count);

	for(std::uint32_t i=0; i < count; ++i)
	{
		glm::vec3 vector;
		vector.x = data.read<float>();
		vector.y = data.read<float>();
		vector.z = data.read<float>();
		cell.vertices.push_back(std::move(vector));
	}
}

void pob_interpreter::_handleINDX(pob_interpreter* pob, anh::ByteBuffer& data)
{
	if(!(pob->cells_.size() > 0))
		return;

	Cell& cell = pob->cells_[pob->cells_.size()-1];

	std::uint32_t count = data.size()/12;
	cell.triangles.reserve(count);

	for(std::uint32_t i=0; i < count; ++i)
	{
		triangle vector;
		vector.a = data.read<std::uint32_t>();
		vector.b = data.read<std::uint32_t>();
		vector.c = data.read<std::uint32_t>();
		cell.triangles.push_back(std::move(vector));
	}
}

void pob_interpreter::_handlePRTL0004(pob_interpreter* pob, anh::ByteBuffer& data)
{
	if(!(pob->cells_.size() > 0))
		return;

	Cell& cell = pob->cells_[pob->cells_.size()-1];

	Link link;

	//I think this is always 1.
	assert(data.read<char>() == 1); 

	link.portal_id = data.read<std::uint32_t>();
	
	//I think this is a normal of some kind.
	link.unkFlag2 = data.read<char>();
	link.dst_cellid = data.read<std::uint32_t>();

	cell.links.push_back(std::move(link));
}

void pob_interpreter::debug()
{
	for(unsigned int i=0; i < cells_.size(); ++i)
	{
		Cell& c = cells_[i];

		printf("==Cell %d==\n", i);
		printf("Name = \"%s\"\n", c.name.c_str());
		//printf("Mesh = \"%s\"\n", c->mesh.c_str());
		//printf("Collision = \"%s\"\n", c->collision.c_str());
		printf("Link Count = %d\n", c.links.size());

		for(unsigned int j=0; j < c.links.size(); ++j)
		{
			Link& link = c.links[j];
			printf("\tLink %d to Cell %d through Portal %d\n", j, link.dst_cellid, link.portal_id);
		}
	}
}