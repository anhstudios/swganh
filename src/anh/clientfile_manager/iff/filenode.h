/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2011 The SWG:ANH Team
---------------------------------------------------------------------------------------
Use of this source code is governed by the GPL v3 license that can be found
in the COPYING file or at http://www.gnu.org/licenses/gpl-3.0.html

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,a
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

#ifndef IFF_FILENODE_H
#define IFF_FILENODE_H

#include "nodes/node.h"
#include <boost/shared_array.hpp>
#include <anh/byte_buffer.h>

namespace anh
{
	namespace clientfile_manager
	{
		namespace iff
		{
			/**

			*/
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
				virtual NODE_TYPE type() {return clientfile_manager::NODE_TYPE_FILE;}

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
		};
	};
};
#endif