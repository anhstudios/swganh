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

#ifndef DATATABLE_INTERPRETER_H
#define DATATABLE_INTERPRETER_H

#include "../interpreter_interface.h"

#include <map>
#include <string>
#include <list>
#include <vector>
#include <cstdint>
#include <boost/any.hpp>

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			class datatable_interpreter : public interpreter_interface
			{
			public:
				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return DATATABLE_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret_data(std::shared_ptr<iff::file_node> node);

				/**
					@brief interprets a IFF::FolderNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret_folder(std::shared_ptr<iff::folder_node> node);

				struct DATA_ROW
				{
					std::vector<boost::any> columns;
				};

				const std::vector<std::string>& column_names() { return column_names_; }
				const std::vector<char>& column_types() { return column_types_; }
					
				std::list<DATA_ROW>::const_iterator begin_itr() { return rows_.cbegin(); }
				std::list<DATA_ROW>::const_iterator end_itr() { return rows_.cend(); }

			private:
				void _handle0001COLS(anh::ByteBuffer& buf);
				void _handleTYPE(anh::ByteBuffer& buf);
				void _handleROWS(anh::ByteBuffer& buf);

				std::vector<char> column_types_;
				std::vector<std::string> column_names_;
				std::list<DATA_ROW> rows_;
			};
		};
	};
};

#endif