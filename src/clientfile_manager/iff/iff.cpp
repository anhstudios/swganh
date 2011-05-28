#include "iff.h"

#include "foldernode.h"
#include "filenode.h"

#include <iostream>
#include <fstream>
#include <stack>

using namespace ClientFileManager::IFF;

IFFFile::IFFFile(std::string filename)
{
	FILE* pFile;
	pFile = fopen(filename.c_str(), "rb");

	if(pFile == nullptr)
	{
		throw IFFNotFound;
	}
	else
	{
		try
		{
			_loadIFF(pFile);
			fclose(pFile);
		} 
		catch(BadFileException& e)
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
	std::stack<std::pair<std::shared_ptr<IFF::Node>, unsigned int>> loader;
	while(!feof(inputstream))
	{
		int arg1 = ftell(inputstream);
		int arg2 = 0;

		if(!loader.empty())
			int arg2 = loader.top().second;

		if(!loader.empty() && arg1 == arg2)
		{
			loader.pop();
		}
		else
		{
			std::shared_ptr<std::string> name = Node::getIFFName(inputstream);

			if(name->size() == 0)
			{
				break;
			}

			std::uint32_t size = readNet<std::uint32_t>(inputstream);
			
			std::cout << ftell(inputstream) << " : " << *name << " " << size << std::endl;
			std::shared_ptr<IFF::Node> node = createNode(name, size);
			
			if(loader.empty())
			{
				this->headNodes.push_back(node);
			}
			else
			{
				IFF::FolderNode* folder = dynamic_cast<IFF::FolderNode*>(loader.top().first.get());
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
				file->loadData(inputstream);
			}
		}

	}
}

std::shared_ptr<ClientFileManager::IFF::Node> IFFFile::createNode(std::shared_ptr<std::string> name, std::uint32_t size)
{
	if	(	*name == "0003DATA" ||
			*name == "PRTSPRTL" ||
			*name == "0000NAME" ||
			*name == "0001NAME" ||
			*name == "0002NAME" ||
			*name == "0003NAME" ||
			*name == "0004NAME" ||
			*name == "0005NAME" ||
			*name == "0006NAME" ||
			*name == "0007NAME" ||
			*name == "0000BASE" ||
			*name == "LGHTCOLR" ||
			*name == "BLADSHDR" ||
			*name == "0005DATA" ||
			*name == "0000VERT" || 
			*name == "PRTL0004" ||
			*name == "0001META" ||
			*name == "ASND" ||
			*name == "RANG" ||
			*name == "TIME"	||
			*name == "DAYN"	||
			*name == "LGTH" ||
			*name == "WDTH"	||
			*name == "OPEN"	||
			*name == "CLOS" ||
			*name == "INDX" ||
			*name == "LGHT" ||
			*name == "PRTL" ||
			*name == "PNOD" ||
			*name == "PEDG" ||
			*name == "ECNT" ||
			*name == "ESTR" ||
			*name == "CRC" ||
			*name == "0003INFO" ||
			*name == "MSGN" ||
			*name == "SKTI" ||
			*name == "LATX" ||
			*name == "SPRT0000" ||
			*name == "0000INFO" ||
			*name == "NAME" ||
			*name == "0004INFO" ||
			*name == "SKTM" ||
			*name == "XFNM" ||
			*name == "POSN" ||
			*name == "TWDH" ||
			*name == "TWDT" ||
			*name == "NORM" ||
			*name == "DOT3" ||
			*name == "OZN " ||
			*name == "PSDTNAME" ||
			*name == "PIDX" ||
			*name == "NIDX" ||
			*name == "TXCI" ||
			*name == "TCSFTCSD" ||
			*name == "PRIMINFO" ||
			*name == "ITL " ||
			*name == "0001SPHR" ||
			*name == "BOX " ||
			*name == "NULL" ||
			*name == "HPTS" ||
			*name == "FLORDATA" ||
			*name == "0001CNT " ||
			*name == "INFO" ||
			*name == "0001INFO" ||
			*name == "0003INFO" ||
			*name == "DATA" ||
			*name == "INDX") 
	{
		return std::shared_ptr<IFF::Node>(new FileNode(name, size));
	}
	else 
	{
		return std::shared_ptr<IFF::Node>(new FolderNode(name, size));
	}
}