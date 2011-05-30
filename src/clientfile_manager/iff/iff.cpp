#include <stack>

#include "iff.h"
#include "nodes/node.h"
#include "filenode.h"
#include "foldernode.h"

#include "interpreters/IFFInterpreter.h"

using namespace ClientFileManager::IFF;

void memflip(void* dst, unsigned int size)
{
	char* buf = reinterpret_cast<char*>(dst);

	unsigned int half = size/2;
	for(unsigned int i=0; i < half; ++i)
	{
		buf[i] ^= buf[size-1-half];
		buf[size-1-half] ^= buf[i];
		buf[i] ^= buf[size-1-half];
	}
};

template <class T> T readNet(FILE* inputstream) 
{
	T t;
	fread(&t, sizeof(T), 1, inputstream);
	memflip(&t, sizeof(T));
	return t;
}

IFFFile::IFFFile(std::string filename, Interpreters::IFFInterpreter* interpreter) throw (NotFoundException)
{
	interpreter_ = interpreter;

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

IFFFile::IFFFile(FILE* inputstream, Interpreters::IFFInterpreter* interpreter)
{
	interpreter_ = interpreter;

	_loadIFF(inputstream);
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

std::string* getIFFName(FILE* input)
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

	return new std::string(name_buffer, namesize);
}

bool childIsNode(FILE* input)
{
	char name_buffer[4];
	fread(name_buffer, 4, 1, input);
	fseek(input, -4, SEEK_CUR);
	return !(getNameSize(name_buffer, 4) < 4);
}

void IFFFile::_loadIFF(FILE* inputstream)
{
	fseek(inputstream, 0, SEEK_END);
	long size = ftell(inputstream);
	fseek(inputstream, 0, SEEK_SET);

	std::stack<std::pair<IFF::Node*, unsigned int>> loader;
	while(ftell(inputstream) < size)
	{
		if(!loader.empty() && ftell(inputstream) == loader.top().second)
		{
			loader.pop();
		}
		else
		{
			std::string* name = getIFFName(inputstream);
			
			if(name->size() == 0 || *name == "NULL") { continue; }
			
			printf("%s\n",name->c_str());
			std::uint32_t size = readNet<std::uint32_t>(inputstream);
		
			IFF::Node* node;
			if(size >= 4 && childIsNode(inputstream))
			{
				//We must be a folder.
				node = new IFF::FolderNode(*name, size);
			}
			else
			{
				//We must be a file.
				IFF::FileNode* file = new IFF::FileNode(*name, size);
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
					loader.push(std::make_pair<IFF::Node*,unsigned int>(node, ftell(inputstream)+size));
				}
			}
			else
			{
				FolderNode* parent = dynamic_cast<FolderNode*>(node);
				if(parent != nullptr)
				{
					parent->add(node);
				}
			}
		}
	}
}