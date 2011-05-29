#ifndef NODE_ECNT_H
#define NODE_ECNT_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_ECNT : public FileNode
			{
			public:
				Node_ECNT(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_ECNT;}

				virtual void loadData(FILE* input)
				{
				}
			};
		};
	};
};

#endif