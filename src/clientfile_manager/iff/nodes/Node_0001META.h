#ifndef NODE_0001META_H
#define NODE_0001META_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_0001META : public FileNode
			{
			public:
				Node_0001META(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_0001META;}

				virtual void loadData(FILE* input)
				{
				}
			};
		};
	};
};

#endif