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

#ifndef INTERPRETERS_WS_H
#define INTERPRETERS_WS_H

#include "../interpreter_interface.h"
#include <cstdint> //for the std::uint32_t and std::uint64_t, ect
#include <vector> //for storing names
#include <map> //For storing chunks
#include <glm/glm.hpp> //For vec3
#include <glm/gtx/quaternion.hpp> //for the quaternions

namespace anh
{
	namespace clientfile_manager
	{
		namespace interpreters
		{
			/**
				@brief An IFFInterpreter for ws files.
			*/
			class ws_interpreter : public interpreter_interface
			{
			public:

				/**
					@brief Constructor for a ws interpreter
				*/
				ws_interpreter();

				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return WS_INTERPRETER; }

				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret(std::shared_ptr<iff::file_node> node);

				/**
					@brief A single chunk object in the world 
				*/
				struct CHUNK
				{
					std::uint64_t id;
					std::uint64_t parent_id;
					std::uint32_t name_id;
					float scale;
					glm::quat orientation;
					glm::vec3 location;
					float object_priority; //could also be type, but I don't buy it.
					std::uint32_t pob_crc;
				};

				const std::map<std::uint64_t, CHUNK>& chunk_map() { return chunks; }

				//This interface should be satisfactory
				size_t name_count() { return names.size(); }
				std::string name(size_t id) { return names[id];}

				/**
					@brief a simple output function for debugging values.
				*/
				void debug();

			private:

				std::map<std::uint64_t, CHUNK> chunks;
				std::vector<std::string> names;

				//Internal Node Handling functions
				void _handleOTNL(anh::ByteBuffer& buffer);
				void _handle0000DATA(anh::ByteBuffer& buffer);
			};
		};
	};
};
#endif