#ifndef NODE_INDX_H
#define NODE_INDX_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_INDX : public FileNode
			{
			public:
				Node_INDX(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_INDX;}

				virtual void loadData(FILE* input)
				{
					std::uint32_t count = size()/12;
					for(std::uint32_t i = 0; i < count; ++i)
					{
						triangles.push_back(readHost<triangle>(input));
					}
				}

				struct triangle
				{
					std::uint32_t a;
					std::uint32_t b;
					std::uint32_t c;
				};

				std::vector<triangle> triangles;
			};
		};
	};
};

#endif