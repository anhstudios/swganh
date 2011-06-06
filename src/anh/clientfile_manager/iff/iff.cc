/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "iff.h"
#include "../notfound_exception.h"
#include "nodes/node.h"
#include "filenode.h"
#include "foldernode.h"
#include "interpreters/interpreter_interface.h"

#include <stack>

using namespace anh::clientfile_manager::iff;

iff_file::iff_file(iff_file&& other)
{
	*this = std::move(other);
}

iff_file& iff_file::operator=(iff_file&& other)
{
	interpreter_ = other.interpreter_;
	headNodes = other.headNodes;
	return *this;
}

iff_file::iff_file(std::string filename, std::shared_ptr<interpreters::interpreter_interface> interpreter)
	: interpreter_(interpreter)
{

	FILE* pFile;
	pFile = fopen(filename.c_str(), "rb");

	if(pFile == nullptr)
	{
		throw anh::clientfile_manager::NotFoundException(filename + " not found!");
	}
	loadIFF_(pFile);
	fclose(pFile);
}

iff_file::iff_file(FILE* inputstream, std::shared_ptr<interpreters::interpreter_interface> interpreter)
	: interpreter_(interpreter)
{
	loadIFF_(inputstream);
}

iff_file::iff_file(anh::ByteBuffer& input, std::shared_ptr<interpreters::interpreter_interface> interpreter)
	: interpreter_(interpreter)
{
	loadIFF_(input);
}

bool isNodeNameCharacter(char c)
{
	if(c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == ' ')
		return true;
	else
		return false;
}

unsigned int getNameSize(char namedata[], unsigned int size)
{
	unsigned int i = 0;
	for(; i < size; ++i)
	{
		if(!isNodeNameCharacter(namedata[i]))
		{
			break;
		}
	}
	return i;
}

std::string getIFFName(anh::ByteBuffer& input)
{
	std::uint64_t name_buf = input.read<std::uint64_t>();
	unsigned int namesize = getNameSize((char*)&name_buf, 8);

	if(namesize < 4) 
	{
		namesize = 0;
		input.read_position(input.read_position()-8);
	}
	else if(namesize < 8)
	{
		namesize = 4;
		input.read_position(input.read_position()-4);
	}
	else 
	{
		namesize = 8;
	}

	return std::string((char*)&name_buf, namesize);
}

bool childIsNode(anh::ByteBuffer& input)
{
	std::uint32_t name_buf = input.read<std::uint32_t>();
	input.read_position(input.read_position()-4);
	return !(getNameSize((char*)&name_buf, 4) < 4);
}

void iff_file::loadIFF_(anh::ByteBuffer& inputstream)
{
	std::stack<std::pair<std::shared_ptr<iff::folder_node>, unsigned int>> loader;
	while(inputstream.read_position() < inputstream.size())
	{
		if(!loader.empty() && inputstream.read_position() == loader.top().second)
		{
			loader.pop();
		}
		else
		{
			std::string name = getIFFName(inputstream);
			
			if(name.size() == 0 || name == "NULL") { continue; }
			
			//printf("%s\n",name->c_str());
			std::uint32_t size = inputstream.read<std::uint32_t>(true);
		
			if(size >= 4 && childIsNode(inputstream))
			{
				//We must be a folder.
				if(loader.empty())
				{
					std::shared_ptr<iff::folder_node> folder = std::make_shared<iff::folder_node>(name, size);
					headNodes.push_back(folder);
					loader.push(std::make_pair<std::shared_ptr<iff::folder_node>,unsigned int>(folder, inputstream.read_position()+size));
				}
				else
				{
					std::shared_ptr<iff::folder_node> folder = std::make_shared<iff::folder_node>(name, size, loader.top().first);
					loader.top().first->add(folder);
					loader.push(std::make_pair<std::shared_ptr<iff::folder_node>,unsigned int>(folder, inputstream.read_position()+size));
				}	
			}
			else
			{
				//We must be a file.
				if(loader.empty())
				{
					std::shared_ptr<iff::file_node> file = std::make_shared<iff::file_node>(name, size);
					file->loadData(inputstream);
					headNodes.push_back(file);
					if(interpreter_ != nullptr)
					{
						interpreter_->interpret(file);
					}
				}
				else
				{
					std::shared_ptr<iff::file_node> file = std::make_shared<iff::file_node>(name, size, loader.top().first);
					file->loadData(inputstream);
					loader.top().first->add(file);
					if(interpreter_ != nullptr)
					{
						interpreter_->interpret(file);
					}
				}
			}
		}
	}
}

void iff_file::loadIFF_(FILE* stream)
{
	unsigned char buffer[1024];
	anh::ByteBuffer theBuf;
	size_t bytesRead = fread(buffer, 1, 1024, stream);
	while(bytesRead == 1024)
	{
		theBuf.write(buffer, bytesRead);
		bytesRead = fread(buffer, 1, 1024, stream);
	}
	theBuf.write(buffer, bytesRead);

	loadIFF_(theBuf);
}