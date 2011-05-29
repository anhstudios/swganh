#include <stack>

#include "iff.h"
#include "swg_nodes.h"

using namespace ClientFileManager::IFF;

NodeIndex IFFFile::index;

IFFFile::IFFFile(std::string filename)
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

IFFFile::IFFFile(FILE* inputstream)
{
	_loadIFF(inputstream);
}

void IFFFile::_loadIFF(FILE* inputstream)
{
	prepIndex();

	fseek(inputstream, 0, SEEK_END);
	long size = ftell(inputstream);
	fseek(inputstream, 0, SEEK_SET);

	std::stack<std::pair<std::shared_ptr<IFF::Node>, unsigned int>> loader;
	while(ftell(inputstream) < size)
	{
		if(!loader.empty() && ftell(inputstream) == loader.top().second)
		{
			loader.pop();
		}
		else
		{
			std::shared_ptr<std::string> name = Node::getIFFName(inputstream);

			if(name->size() == 0)
			{
				throw new IFF::BadIFFException("FATAL: A File node was mistaken for a folder.");
			}
			else if(*name == "NULL")
			{
				continue;
			}

			std::uint32_t size = readNet<std::uint32_t>(inputstream);
			
			//printf("%d : %s : %d\n", ftell(inputstream), name->c_str(), size);
			std::shared_ptr<IFF::Node> node = createNode(name, size);
			
			if(loader.empty())
			{
				this->headNodes.push_back(node);
			}
			else
			{
				IFF::FolderNode* folder = dynamic_cast<IFF::FolderNode*>(loader.top().first.get());

				if(folder == nullptr)
				{
					throw new IFF::BadIFFException("FATAL: The stack contained a file when it MUST contain a folder");
				}

				folder->add(node);
			}

			if(node->type() == ClientFileManager::NODE_TYPE_FOLDER)
			{
				//FOLDER
				loader.push(std::make_pair<std::shared_ptr<IFF::Node>, unsigned int>(node, ftell(inputstream) + size));
			}
			else
			{
				//FILE
				IFF::FileNode* file = dynamic_cast<IFF::FileNode*>(node.get());

				if(file == nullptr)
				{
					throw new IFF::BadIFFException("FATAL: The node we thought was a file was not 0_o.");
				}

				file->loadData(inputstream);
			}
		}
	}
}

void IFFFile::prepIndex() {
	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"0003DATA", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new Nodes::Node_0003DATA(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"PRTSPRTL", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new Nodes::Node_PRTL(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"PRTL", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new Nodes::Node_PRTL(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"0005DATA", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
		return std::shared_ptr<IFF::Node>(new Nodes::Node_0005DATA(name, size));
	}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"0000VERT", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new Nodes::Node_0000VERT(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"INDX", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new Nodes::Node_INDX(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"0001META", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new FileNode(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"PNOD", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new FileNode(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"PEDG", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new FileNode(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"ECNT", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new FileNode(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"ESTR", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new FileNode(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"PRTL0004", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new Nodes::Node_PRTL0004(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"LGHT", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new FileNode(name, size));
		}));

	index.insert(std::make_pair<std::string, IFFCreatorCallback>(
		"CRC ", [] (std::shared_ptr<std::string> name, std::uint32_t size) -> std::shared_ptr<IFF::Node> {
			return std::shared_ptr<IFF::Node>(new FileNode(name, size));
		}));
}

std::shared_ptr<ClientFileManager::IFF::Node> IFFFile::createNode(std::shared_ptr<std::string> name, std::uint32_t size)
{
	NodeIndexIterator it = index.find(*name);
	if(it != index.end())
	{
		return it->second(name, size);
	}
	else 
	{
		return std::shared_ptr<IFF::Node>(new FolderNode(name, size));
	}
}