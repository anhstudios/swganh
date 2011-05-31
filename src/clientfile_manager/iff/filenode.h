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

			virtual void loadData(anh::ByteBuffer& input)
			{
				data_.write(input.data()+input.read_position(), size());
				input.read_position(input.read_position()+size());
			}

			anh::ByteBuffer& data() { return data_; }

		private:
			anh::ByteBuffer data_;
		};
	};
};

#endif