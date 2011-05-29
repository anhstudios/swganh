#ifndef NODE_PNOD_H
#define NODE_PNOD_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_PNOD : public FileNode
			{
			public:
				Node_PNOD(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_PNOD;}

				virtual void loadData(FILE* input)
				{
				}
			};
		};
	};
};

#endif