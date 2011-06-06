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

#ifndef IFF_H
#define IFF_H

#include "interpreters/interpreter_interface.h"

#include <stdio.h>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>
#include <functional>

#include <anh/byte_buffer.h>

namespace anh
{
	namespace clientfile_manager
	{
		namespace iff
		{
			class node;

			class iff_file
			{
			public:
				//Constructors
				iff_file(iff_file&& other);
				iff_file(const std::string filename, std::shared_ptr<interpreters::interpreter_interface> interpret=nullptr);
				iff_file(FILE* inputstream, std::shared_ptr<interpreters::interpreter_interface> interpret=nullptr);
				iff_file(anh::ByteBuffer& input, std::shared_ptr<interpreters::interpreter_interface> interpret=nullptr);

				/**
					@return the interpreter associated with this IFFFile
				*/
				std::shared_ptr<interpreters::interpreter_interface> interpreter() { return interpreter_;}
			
				/**
					@param i the index of the headnode to remove
					@return a head node
				*/
				std::shared_ptr<clientfile_manager::iff::node> headnode(unsigned int i) {return headNodes[i];}

				/**
					@return the count of headnodes
				*/
				unsigned int headnode_count() {return headNodes.size();}

				//Move constructor
				iff_file& operator=(iff_file&& other);
			protected:
			private:
				void loadIFF_(FILE* stream);
				void loadIFF_(anh::ByteBuffer& inputstream);
				std::vector<std::shared_ptr<clientfile_manager::iff::node>> headNodes;
				std::shared_ptr<interpreters::interpreter_interface> interpreter_;
			};
		};
	};
};
#endif