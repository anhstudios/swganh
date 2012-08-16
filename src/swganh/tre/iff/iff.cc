// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "iff.h"
#include "node.h"
#include "filenode.h"
#include "foldernode.h"

#include <stack>

using namespace swganh::tre;

iff_file::iff_file(anh::ByteBuffer& input, std::shared_ptr<VisitorInterface> visitor)
	: visitor_(visitor)
{
	loadIFF_(input);
}

iff_file& iff_file::operator=(iff_file&& other)
{
	visitor_ = other.visitor_;
	headNodes = other.headNodes;
	return *this;
}

void iff_file::loadIFF_(anh::ByteBuffer& inputstream)
{
	//We use a stack instead of recursion to make things more straightforward to follow.
	std::stack<std::pair<std::shared_ptr<folder_node>, unsigned int>> loader;
	int depth = 1;

	//While the stream is not finished
	while(inputstream.read_position() < inputstream.size())
	{
		//We check to see if our read_position has exited a folder node on the stack
		if(!loader.empty() && inputstream.read_position() == loader.top().second)
		{
			//It has, so we pop the folder node off the stack
			loader.pop();
			--depth;
		}
		else
		{
			//It hasn't, so we get the name of the next node in the stream
			std::string name = getIFFName_(inputstream);
			
			//It wasn't an iff, or it had an edge case's name. We continue from here as normal
			if(name.size() == 0 || name == "NULL") { continue; }
			
			//It was, so we read in the size information
			std::uint32_t size = inputstream.read<std::uint32_t>(true);
			
			//Then we check to see if what it contains is a node or data based the first eight characters
			if(size >= 4 && isFolderNode_(name))
			{
				//We must be a folder.
				if(loader.empty())
				{
					//The loader is empty, so we don't have a parent, and we need to be pushed back on to the head nodes instead of our parent's child list
					
					loader.push(std::make_pair<std::shared_ptr<folder_node>,unsigned int>(
						std::make_shared<folder_node>(name, size),
							inputstream.read_position()+size));

					std::shared_ptr<folder_node> folder(loader.top().first);
					headNodes.push_back(folder);

					if(visitor_ != nullptr)
					{
						visitor_->visit_folder(depth++, folder);
					}
				}
				else
				{
					//The loader has something, so we have a parent we need to register with
					auto oldTop = loader.top().first;
					loader.push(std::make_pair<std::shared_ptr<folder_node>,unsigned int>(
						std::make_shared<folder_node>(name, size, loader.top().first), 
						inputstream.read_position()+size));

					std::shared_ptr<folder_node> folder(loader.top().first);
					oldTop->add(folder);
					if(visitor_ != nullptr)
					{
						visitor_->visit_folder(depth++, folder);
					}
				}	
			}
			else
			{
				//We must be a file.
				if(loader.empty())
				{
					//The loader has nothing. This is weird for a file node, but we'll try and handle it anyway
					std::shared_ptr<file_node> file = std::make_shared<file_node>(name, size);
					file->loadData(inputstream);
					headNodes.push_back(file);

					//If we hae an interpreter we will have it interpret our data
					if(visitor_ != nullptr)
					{
						visitor_->visit_data(depth, file);
					}
				}
				else
				{
					//The loader has a folder for us to register with
					std::shared_ptr<file_node> file = std::make_shared<file_node>(name, size, loader.top().first);
					file->loadData(inputstream);
					loader.top().first->add(file);

					//If we have an interpreter we wil have it interpret our data
					if(visitor_ != nullptr)
					{
						visitor_->visit_data(depth, file);
					}
				}
			}
		}
	}
}

std::string iff_file::getIFFName_(anh::ByteBuffer& input)
{
	//Lets read ahead 8 bytes. We could peek here, but we'll need to adjust our stream position anyway
	//so lets just read.
	std::uint64_t name_buf = input.read<std::uint64_t>();
	
	//Get the size of the name we just read from the stream
	unsigned int namesize = getNameSize_((char*)&name_buf, 8);

	if(namesize < 4) 
	{
		//The name size is 0. Lets backtrack and hope for the best
		namesize = 0;
		input.read_position_delta(-8);
	}
	else if(namesize < 8)
	{
		//The name size is 4. Lets backtrack 4 bytes to keep other things happy.
		namesize = 4;
		input.read_position_delta(-4);
	}
	else 
	{
		//The name size is 8.
		namesize = 8;
	}

	//Return the string we found
	return std::string((char*)&name_buf, namesize);
}

bool iff_file::isNodeNameCharacter_(char c)
{
	//Returns true if the character is within the proper range for an iff node name
	//in regex : [A-Z0-9 ]
	if(c >= 'A' && c <= 'Z' || c >= '0' && c <= '9' || c == ' ')
		return true;
	else
		return false;
}

unsigned int iff_file::getNameSize_(char namedata[], unsigned int size)
{
	//Lets get the name size based on this namedata array
	unsigned int i = 0;
	while(i < size)
	{
		if(!isNodeNameCharacter_(namedata[i]))
		{
			break;
		}
		++i;
	}

	//Return what we think the name size is
	return i;
}

bool iff_file::isFolderNode_(std::string& name)
{
	if(name.substr(0, 4) == "FORM" || name.substr(4, 8) == "FORM")
	{
		return true;
	}
	return false;
}