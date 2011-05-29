#ifndef NODE_PEDG_H
#define NODE_PEDG_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_PEDG : public FileNode
			{
			public:
				Node_PEDG(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_PEDG;}

				virtual void loadData(FILE* input)
				{
				}
			};
		};
	};
};

#endif