#ifndef NODE_PRTL0004_H
#define NODE_PRTL0004_H

#include "filenode.h"

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Nodes
		{
			class Node_PRTL0004 : public FileNode
			{
			public:
				Node_PRTL0004(std::shared_ptr<std::string> name, std::uint32_t size)
					: FileNode(name, size)
				{
				}

				virtual NODE_TYPE type() { return ClientFileManager::NODE_TYPE_PRTL0004;}

				virtual void loadData(FILE* input)
				{
					unkFlag1 = fgetc(input);
					cell1 = readHost<std::uint32_t>(input);

					unkFlag2 = fgetc(input);
					cell2 = readHost<std::uint32_t>(input);

					door = readHost<std::string>(input);

					unkFlag3 = fgetc(input);
					first = readHost<unk>(input);
					second = readHost<unk>(input);
					third = readHost<unk>(input);
				}

				struct unk
				{
					float a;
					float b;
					float c;
					float d;
				};

				char unkFlag1;
				std::uint32_t cell1;
				
				char unkFlag2;
				std::uint32_t cell2;

				std::string door;

				char unkFlag3;
				unk first;
				unk second;
				unk third;

			};
		};
	};
};

#endif