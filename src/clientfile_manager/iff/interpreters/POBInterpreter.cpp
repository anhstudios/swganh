#include "POBInterpreter.h"

#include "../iff.h"
#include "../filenode.h"

using namespace ClientFileManager::IFF::Interpreters;

void POBInterpreter::interpret(ClientFileManager::IFF::FileNode* node)
{
	if(node->name() == "0003DATA")
	{
		_handle0003DATA(node->data());
	}
	else if(node->name() == "PRTL" || node->name() == "PRTSPRTL")
	{
		_handlePRTL(node->data());
	}
	else if(node->name() == "0005DATA")
	{
		_handle0005DATA(node->data());
	}
	else if(node->name() == "0000VERT")
	{
		_handle0000VERT(node->data());
	}
	else if(node->name() == "INDX")
	{
		_handleINDX(node->data());
	}
	else if(node->name() == "PRTL0004")
	{
		_handlePRTL0004(node->data());
	}
	else if(node->name() == "LGHT")
	{
		//_handleLGHT(node->data());
	}
	else if(node->name() == "0001META")
	{
		//_handle0001META(node->data());
	}
	else if(node->name() == "PNOD")
	{
		//_handlePNOD(node->data());
	}
	else if(node->name() == "PEDG")
	{
		//_handlePEDG(node->data());
	}
	else if(node->name() == "ECNT")
	{
		//_handleECNT(node->data());
	}
	else if(node->name() == "ESTR")
	{
		//_handleESTR(node->data());
	}
	else if(node->name() == "CRC ")
	{
		//_handleCRC_(node->data());
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
	Portal* newP = new Portal();

	std::uint32_t size = data.read<std::uint32_t>();
	newP->vertices.reserve(size);
	for(std::uint32_t i = 0; i < size; ++i)
	{
		glm::vec3* vector = new glm::vec3();
		vector->x = data.read<float>();
		vector->y = data.read<float>();
		vector->z = data.read<float>();
		newP->vertices.push_back(vector);
	}

	portals_.push_back(newP);
}

void POBInterpreter::_handle0005DATA(anh::ByteBuffer& data)
{
	Cell* cell = new Cell();
	cell->links.reserve(data.read<std::uint32_t>());
	cell->unkFlag2 = data.read<char>();
	cell->name = readStringFromByteBuffer(data);
	cell->mesh = readStringFromByteBuffer(data);
	cell->unkFlag3 = data.read<char>();
	cell->collision = readStringFromByteBuffer(data);
	cells_.push_back(cell);
}

void POBInterpreter::_handle0000VERT(anh::ByteBuffer& data)
{
	Cell* cell = cells_[cells_.size()-1];
	
	std::uint32_t count = data.size()/12;
	cell->vertices.reserve(count);

	for(std::uint32_t i=0; i < count; ++i)
	{
		glm::vec3* vector = new glm::vec3();
		vector->x = data.read<float>();
		vector->y = data.read<float>();
		vector->z = data.read<float>();
		cell->vertices.push_back(vector);
	}
}

void POBInterpreter::_handleINDX(anh::ByteBuffer& data)
{
	Cell* cell = cells_[cells_.size()-1];

	std::uint32_t count = data.size()/12;
	cell->triangles.reserve(count);

	for(std::uint32_t i=0; i < count; ++i)
	{
		triangle* vector = new triangle();
		vector->a = data.read<std::uint32_t>();
		vector->b = data.read<std::uint32_t>();
		vector->c = data.read<std::uint32_t>();
		cell->triangles.push_back(vector);
	}
}

void POBInterpreter::_handlePRTL0004(anh::ByteBuffer& data)
{
	Cell* cell = cells_[cells_.size()-1];

	Link* link = new Link();

	//I think this is always 1.
	assert(data.read<char>() == 1); 

	link->portal_id = data.read<std::uint32_t>();
	
	//I think this is a normal of some kind.
	link->unkFlag2 = data.read<char>();
	link->dst_cellid = data.read<std::uint32_t>();

	cell->links.push_back(link);
}

void POBInterpreter::debug()
{
	for(unsigned int i=0; i < cells_.size(); ++i)
	{
		Cell* c = cells_[i];

		printf("==Cell %d==\n", i);
		printf("Name = \"%s\"\n", c->name.c_str());
		//printf("Mesh = \"%s\"\n", c->mesh.c_str());
		//printf("Collision = \"%s\"\n", c->collision.c_str());
		printf("Link Count = %d\n", c->links.size());

		for(unsigned int j=0; j < c->links.size(); ++j)
		{
			Link* link = c->links[j];
			printf("\tLink %d to Cell %d through Portal %d\n", j, link->dst_cellid, link->portal_id);
		}

	}

}