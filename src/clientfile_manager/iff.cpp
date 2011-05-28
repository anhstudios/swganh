#include "iff.h"

#include "foldernode.h"
#include "filenode.h"

#include <iostream>
#include <fstream>
#include <stack>

using namespace ClientFileManager::IFF;

IFFFile::IFFFile(std::string filename)
{
	std::ifstream input;
	input.open(filename);

	if(input.fail())
	{
		throw IFFNotFound;
	}
	else
	{
		try
		{
			_loadIFF(input);
		} 
		catch(BadFileException& e)
		{
			input.close();
			throw e;
		}
	}
}

IFFFile::IFFFile(std::istream& inputstream)
{
	_loadIFF(inputstream);
}

void IFFFile::_loadIFF(std::istream& inputstream)
{
	std::stack<std::pair<boost::shared_ptr<IFF::Node>, unsigned int>> loader;
	while(inputstream.good())
	{
		if(!loader.empty() && loader.top().second > inputstream.tellg())
		{
			loader.pop();
		}
		else
		{
			boost::shared_ptr<std::string> name = Node::getIFFName(inputstream);
			if(name->size() > 0)
			{
				std::cout << name << std::endl;
				//Parent must be a folder node
				unsigned int size = readNet<unsigned int>(inputstream);
				boost::shared_ptr<IFF::Node> node = createNode(name, size);

				if(!loader.empty())
				{
					boost::shared_ptr<IFF::Node> parent = loader.top().first;

					IFF::FolderNode* f = dynamic_cast<IFF::FolderNode*>(loader.top().first.get());
					if(f == nullptr)
					{
						throw BadIFF;
					}

					f->add(node);
				} else {
					headNodes.push_back(node);
				}

				int cur = inputstream.tellg();
				loader.push(std::make_pair<boost::shared_ptr<IFF::Node>, unsigned int>(node,cur+size));
			}
			else
			{
				//Parent must be a data node.
				IFF::FileNode* f = dynamic_cast<IFF::FileNode*>(loader.top().first.get());
				
				if(f == nullptr)
				{
					throw BadIFF;
				}

				f->loadData(inputstream, f->size());
			}
		}

	}
}

boost::shared_ptr<ClientFileManager::IFF::Node> IFFFile::createNode(boost::shared_ptr<std::string> name, unsigned int size)
{
	return boost::shared_ptr<IFF::Node>(new FolderNode(name, size));
}