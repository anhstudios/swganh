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

#include "../IFFInterpreter.h"
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ClientFileManager
{
	namespace IFF
	{
		namespace Interpreters
		{
			class WSInterpreter : public IFFInterpreter
			{
			public:
				
				WSInterpreter();

				virtual void interpret(std::shared_ptr<IFF::FileNode> node);

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

				std::map<std::uint64_t, CHUNK> chunks;
				std::vector<std::string> names;
				
				void debug();
				virtual InterpreterType getType() { return WS_INTERPRETER; }

			private:
				void _handleOTNL(anh::ByteBuffer& buffer);
				void _handle0000DATA(anh::ByteBuffer& buffer);
			};
		};
	};
};

#endif