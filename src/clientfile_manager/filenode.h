#ifndef IFF_FILENODE_H
#define IFF_FILENODE_H

#include "node.h"

namespace ClientFileManager
{
	namespace IFF
	{
		class FileNode : public Node
		{
		public:
			FileNode(boost::shared_ptr<std::string> name, unsigned int size)
				: Node(name, size), data_(nullptr)
			{
			}

			virtual void loadData(std::istream& input, unsigned int size)
			{
				char* buf = new char[size];
				input >> buf;

				data_ = boost::shared_array<char>(buf);
			}

			boost::shared_array<char> data() { return data_; }

		private:
			boost::shared_array<char> data_;
		};
	};
};

#endif