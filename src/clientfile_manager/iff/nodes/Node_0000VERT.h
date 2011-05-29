#ifndef NODE_0000VERT_H
#define NODE_0000VERT_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_0000VERT : public FileNode
			{
			public:
				Node_0000VERT(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_0000VERT;}

				virtual void loadData(FILE* input)
				{
					std::uint32_t count = size()/12;
					for(std::uint32_t i = 0; i < count; ++i)
					{
						vertices.push_back(readHost<vertex>(input));
					}

				}

				struct vertex
				{
					float x;
					float y;
					float z;
				};

				std::vector<vertex> vertices;
			};
		};
	};
};

#endif