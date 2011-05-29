#ifndef NODE_0005DATA_H
#define NODE_0005DATA_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_0005DATA : public FileNode
			{
			public:
				Node_0005DATA(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_0005DATA;}

				virtual void loadData(FILE* input)
				{
					
					unk1 = readHost<std::uint32_t>(input);
					unkFlag2 = fgetc(input);
					name = readHost<std::string>(input);
					appearance = readHost<std::string>(input);
					
					unkFlag3 = fgetc(input);
					
					collision = readHost<std::string>(input);
				}

				std::uint32_t unk1;
				char unkFlag2;
				std::string name;
				std::string appearance;

				char unkFlag3;

				std::string collision;
			};
		};
	};
};

#endif