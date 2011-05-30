#ifndef IFF_FILENODE_H
#define IFF_FILENODE_H

#include "nodes/node.h"
#include <boost/shared_array.hpp>
#include <anh/byte_buffer.h>

namespace ClientFileManager
{
	namespace IFF
	{
		class FileNode : public Node
		{
		public:
			FileNode(std::string& name, unsigned int size)
				: Node(name, size), data_(size)
			{
			}

			virtual NODE_TYPE type() {return ClientFileManager::NODE_TYPE_FILE;}

			virtual void loadData(FILE* input)
			{
				char* buf = new char[size()];
				fread(buf, size(), 1, input);
				data_.write(buf);
				delete[] buf;
			}

			anh::ByteBuffer& data() { return data_; }

		private:
			anh::ByteBuffer data_;
		};
	};
};

#endif