#include "node.h"

using namespace ClientFileManager::IFF;

unsigned int Node::getNameSize(char namedata[], unsigned int size)
{
	unsigned int i = 0;
	for(; i < size; ++i)
	{
		if(!ClientFileManager::IFF::Node::isNodeNameCharacter(namedata[i]))
		{
			break;
		}
	}
	return i;
}

boost::shared_ptr<std::string> Node::getIFFName(std::istream& input)
{
	char name_buffer[8];
	input >> name_buffer;

	unsigned int namesize = getNameSize(name_buffer, 8);
	if(namesize < 4) 
	{
		namesize = 0;
		input.seekg(-8, std::ios_base::cur);
	}
	else if(namesize < 8)
	{
		namesize = 4;
		input.seekg(-4, std::ios_base::cur);
	}
	else 
	{
		namesize = 8;
	}

	return boost::shared_ptr<std::string>(new std::string(name_buffer, namesize));
}

ClientFileManager::IFF::Node::Node(std::istream& input)
{
	name_ = getIFFName(input);
	size_ = readNet<unsigned int>(input);
}