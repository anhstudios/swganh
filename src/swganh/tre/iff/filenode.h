// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef IFF_FILENODE_H
#define IFF_FILENODE_H

#include "node.h"
#include <boost/shared_array.hpp>
#include <anh/byte_buffer.h>

namespace swganh
{
namespace tre
{
	class file_node : public node
	{
	public:

		/**
			@brief Constructor for a FileNode
		*/
		file_node(std::string& name, unsigned int size)
			: node(name, size), data_()
		{
			data_.reserve(size);
		}

		file_node(std::string& name, unsigned int size, std::shared_ptr<node> parent)
			: node(name, size, parent), data_()
		{
			data_.reserve(size);
		}

		/**
			@return the node type of this node.
		*/
		virtual NODE_TYPE type() {return NODE_TYPE_FILE;}

		/**
			@brief copies the data from the input bytebuffer into this node's internal byte buffer.
		*/
		virtual void loadData(anh::ByteBuffer& input)
		{
			data_.write(input.data()+input.read_position(), size());
			input.read_position_delta(size());
		}

		/**
				
		*/
		anh::ByteBuffer& data() { return data_; }

	private:
		file_node();

		anh::ByteBuffer data_;
	};
}
}
#endif