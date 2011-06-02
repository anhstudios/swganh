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

#ifndef INTERPRETERS_OIFF_INTERPRETER_H
#define INTERPRETERS_OIFF_INTERPRETER_H

#include "../IFFInterpreter.h"

#include <map>
#include <vector>
#include <cstdint>
#include <memory>

namespace ClientFileManager
{
	class FileManager;

	namespace IFF
	{
		namespace Interpreters
		{
			class OIFFInterpreter;

			typedef std::function<void(OIFFInterpreter*, std::string&, anh::ByteBuffer&)> AttributeFunctor;
			typedef std::map<std::string, AttributeFunctor> AttributeHandlerIndex;
			typedef std::map<std::string, AttributeFunctor>::const_iterator AttributeHandlerIndexIterator;

			class OIFFInterpreter : public IFFInterpreter
			{
			public:

				OIFFInterpreter();
				virtual InterpreterType getType() { return OIFF_INTERPRETER; }
				virtual void interpret(std::shared_ptr<IFF::FileNode> node);

				std::string getStringAttribute(std::string& key)
				{
					std::map<std::string, std::string>::const_iterator it = stringAttributes.find(key);
					if(it != stringAttributes.cend())
					{
						return it->second;
					}
					else
					{
						return "";
					}

				}

				std::uint32_t getIntAttribute(std::string& key)
				{
					std::map<std::string, std::uint32_t>::const_iterator it = intAttributes.find(key);
					if(it != intAttributes.cend())
					{
						return it->second;
					}
					else
					{
						return 0;
					}

				}

				float getFloatAttribute(std::string& key)
				{
					std::map<std::string, float>::const_iterator it = floatAttributes.find(key);
					if(it != floatAttributes.cend())
					{
						return it->second;
					}
					else
					{
						return 0.0;
					}

				}

				bool getBoolAttribute(std::string key)
				{
					std::map<std::string, bool>::const_iterator it = boolAttributes.find(key);
					if(it != boolAttributes.cend())
					{
						return it->second;
					}
					else
					{
						return false;
					}

				}

				void loadAggregateData(FileManager& f);

			private:

				static AttributeHandlerIndex attributeHandler_;

				static void _handleString(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleInt(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleFloat(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleBool(OIFFInterpreter* dst, std::string& name, anh::ByteBuffer& buf);

				void _handleXXXX(anh::ByteBuffer& buf);
				void _handleDERVXXXX(anh::ByteBuffer& buf);
				void _handlePCNT(anh::ByteBuffer& buf);

				std::map<std::string, std::uint32_t> intAttributes;
				std::map<std::string, std::string> stringAttributes;
				std::map<std::string, float> floatAttributes;
				std::map<std::string, bool> boolAttributes;

				std::vector<std::string> parentFiles;
			};
		};
	};
};

#endif