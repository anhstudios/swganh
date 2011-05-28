#ifndef IFF_FILENODE_H
#define IFF_FILENODE_H

#include "node.h"
#include <boost/shared_array.hpp>

namespace ClientFileManager
{
	namespace IFF
	{
		class FileNode : public Node
		{
		public:
			FileNode(std::shared_ptr<std::string> name, unsigned int size)
				: Node(name, size), data_(nullptr)
			{
			}

			virtual void loadData(FILE* input)
			{
				char* buf = new char[size()];
				fread(buf, size(), 1, input);
				data_ = boost::shared_array<char>(buf);
			}

			boost::shared_array<char> data() { return data_; }

		private:
			boost::shared_array<char> data_;
		};
	};
};

#endif