#ifndef NODE_PRTL_H
#define NODE_PRTL_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_PRTL : public FileNode
			{
			public:
				Node_PRTL(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_PRTL;}

				virtual void loadData(FILE* input)
				{
					vertex_count = readHost<std::uint32_t>(input);
					for(std::uint32_t i=0; i < vertex_count; ++i)
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

				std::uint32_t getVertexCount() {return vertex_count;}
				vertex* getVertex(std::uint32_t i) { return &vertices[i]; }

			private:

				std::uint32_t vertex_count;
				std::vector<vertex> vertices;
			};
		};
	};
};

#endif