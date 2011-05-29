#ifndef NODE_ESTR_H
#define NODE_ESTR_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_ESTR : public FileNode
			{
			public:
				Node_ESTR(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_ESTR;}

				virtual void loadData(FILE* input)
				{
				}
			};
		};
	};
};

#endif