#include "node.h"

#include <cstdint>

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

std::shared_ptr<std::string> Node::getIFFName(FILE* input)
{
	char name_buffer[8];
	fread(name_buffer, 8, 1, input);

	unsigned int namesize = getNameSize(name_buffer, 8);

	if(namesize < 4) 
	{
		namesize = 0;
		fseek(input, -8L, SEEK_CUR);
	}
	else if(namesize < 8)
	{
		namesize = 4;
		fseek(input, -4L, SEEK_CUR);
	}
	else 
	{
		namesize = 8;
	}

	return std::shared_ptr<std::string>(new std::string(name_buffer, namesize));
}