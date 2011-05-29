#ifndef NODE_0003DATA_H
#define NODE_0003DATA_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_0003DATA : public FileNode
			{
			public:
				Node_0003DATA(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_0003DATA;}

				virtual void loadData(FILE* input)
				{
					portal_count = readHost<std::uint32_t>(input);
					cell_count = readHost<std::uint32_t>(input);
				}

				std::uint32_t portal_count;
				std::uint32_t cell_count;
			};
		};
	};
};

#endif