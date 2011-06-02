#include "POBInterpreter.h"

#include "../../iff.h"
#include "../../filenode.h"

using namespace ClientFileManager::IFF::Interpreters;
using namespace std::tr1::placeholders;
using namespace std;
using namespace boost;

POBInterpreter::POBInterpreter()
	: IFFInterpreter()
{
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("INDX"), bind(&POBInterpreter::_handleINDX, this, _1)));
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("0003DATA"), bind(&POBInterpreter::_handle0003DATA, this, _1)));
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("0005DATA"), bind(&POBInterpreter::_handle0005DATA, this, _1)));
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("PRTL"), bind(&POBInterpreter::_handlePRTL, this, _1)));
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("PRTSPRTL"), bind(&POBInterpreter::_handlePRTL, this, _1)));
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("PRTL0004"), bind(&POBInterpreter::_handlePRTL0004, this, _1)));
	nameLookup_.insert(make_pair<string, NodeFunctor>(string("0000VERT"), bind(&POBInterpreter::_handle0000VERT, this, _1)));
}

void POBInterpreter::interpret(std::shared_ptr<ClientFileManager::IFF::FileNode> node)
{
	NodeNameIndexIterator it = nameLookup_.find(node->name());
	if( it != nameLookup_.cend())
	{
		it->second(node->data());
	}
}

void POBInterpreter::_handle0003DATA(anh::ByteBuffer& data)
{
	int test = data.read<std::uint32_t>();

	portals_.reserve(test);
	cells_.reserve(data.read<std::uint32_t>());
}

void POBInterpreter::_handlePRTL(anh::ByteBuffer& data)
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

	portals_.push_back(newP);
}

void POBInterpreter::_handle0005DATA(anh::ByteBuffer& data)
{
	Cell cell;
	cell.links.reserve(data.read<std::uint32_t>());
	cell.unkFlag2 = data.read<char>();
	cell.name = readStringFromByteBuffer(data);
	cell.mesh = readStringFromByteBuffer(data);
	cell.unkFlag3 = data.read<char>();
	cell.collision = readStringFromByteBuffer(data);
	cells_.push_back(std::move(cell));
}

void POBInterpreter::_handle0000VERT(anh::ByteBuffer& data)
{
	Cell& cell = cells_[cells_.size()-1];
	
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

void POBInterpreter::_handleINDX(anh::ByteBuffer& data)
{
	Cell& cell = cells_[cells_.size()-1];

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

void POBInterpreter::_handlePRTL0004(anh::ByteBuffer& data)
{
	Cell& cell = cells_[cells_.size()-1];

	Link link;

	//I think this is always 1.
	assert(data.read<char>() == 1); 

	link.portal_id = data.read<std::uint32_t>();
	
	//I think this is a normal of some kind.
	link.unkFlag2 = data.read<char>();
	link.dst_cellid = data.read<std::uint32_t>();

	cell.links.push_back(std::move(link));
}

void POBInterpreter::debug()
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