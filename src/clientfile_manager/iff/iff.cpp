#include <stack>

#include "iff.h"
#include "nodes/node.h"
#include "filenode.h"
#include "foldernode.h"

#include "interpreters/IFFInterpreter.h"

using namespace ClientFileManager::IFF;

IFFFile::IFFFile(IFFFile&& other)
{
	*this = std::move(other);
}

IFFFile& IFFFile::operator=(IFFFile&& other)
{
	interpreter_ = other.interpreter_;
	headNodes = other.headNodes;
	return *this;
}

IFFFile::IFFFile(std::string filename, std::shared_ptr<Interpreters::IFFInterpreter> interpreter)
	: interpreter_(interpreter)
{

	FILE* pFile;
	pFile = fopen(filename.c_str(), "rb");

	if(pFile == nullptr)
	{
		throw new NotFoundException(filename + " not found!");
	}
	else
	{
		try
		{
			_loadIFF(pFile);
			fclose(pFile);
		} 
		catch(BadIFFException& e)
		{
			fclose(pFile);
			throw e;
		}
	}
}

IFFFile::IFFFile(FILE* inputstream, std::shared_ptr<Interpreters::IFFInterpreter> interpreter)
	: interpreter_(interpreter)
{
	_loadIFF(inputstream);
}

IFFFile::IFFFile(anh::ByteBuffer& input, std::shared_ptr<Interpreters::IFFInterpreter> interpreter)
	: interpreter_(interpreter)
{
	_loadIFF(input);
}

bool isNodeNameCharacter(char c)
{
	if(c >= 48 && c <= 57 || c >= 65 && c <= 90 || c == 32)
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

void IFFFile::_loadIFF(anh::ByteBuffer& inputstream)
{
	std::stack<std::pair<std::shared_ptr<IFF::Node>, unsigned int>> loader;
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
		
			std::shared_ptr<IFF::Node> node;
			if(size >= 4 && childIsNode(inputstream))
			{
				//We must be a folder.
				node = std::make_shared<IFF::FolderNode>(name, size);
			}
			else
			{
				//We must be a file.
				std::shared_ptr<IFF::FileNode> file = std::make_shared<IFF::FileNode>(name, size);
				file->loadData(inputstream);
				node = file;
			
				if(interpreter_ != nullptr)
				{
					interpreter_->interpret(file);
				}
			}
			
			//Setup Parent
			if(loader.empty())
			{
				headNodes.push_back(node);
				if(node->type() == NODE_TYPE_FOLDER)
				{
					loader.push(std::make_pair<std::shared_ptr<IFF::Node>,unsigned int>(node, inputstream.read_position()+size));
				}
			}
			else
			{
				FolderNode* parent = dynamic_cast<FolderNode*>(node.get());
				if(parent != nullptr)
				{
					parent->add(node);
				}
			}
		}
	}
}

void IFFFile::_loadIFF(FILE* stream)
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

	_loadIFF(theBuf);
}