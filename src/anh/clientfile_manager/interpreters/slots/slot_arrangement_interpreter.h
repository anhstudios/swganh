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

#ifndef SLOT_ARRANGEMENT_INTERPRETER_H
#define SLOT_ARRANGEMENT_INTERPRETER_H

#include "../interpreter_interface.h"

#include <vector>
#include <list>

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			class slot_arrangement_interpreter : public interpreter_interface
			{
			public:
				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return SLOT_ARRANGEMENT_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret(std::shared_ptr<iff::file_node> node);

				//const std::vector<std::string>& slots_occupied(size_t id) { return combinations_[id]; };

				std::list<std::vector<std::string>>::const_iterator begin() {return combinations_.cbegin();}
				std::list<std::vector<std::string>>::const_iterator end() {return combinations_.cend();}

			private:
				void _handle0000ARG(anh::ByteBuffer& buf);

				std::list<std::vector<std::string>> combinations_;

			};
		};
	};
};

#endif