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

#include "../interpreter_interface.h"

#include <map>
#include <unordered_map>
#include <set>
#include <functional>
#include <cstdint>
#include <memory>
#include <boost/any.hpp>

namespace anh
{
	namespace clientfile_manager
	{
		//Forward Declarations
		class file_manager;

		namespace interpreters
		{
			class oiff_interpreter;
			//End Forward Declarations

			//Begin Typedefs
			typedef std::function<void(oiff_interpreter*, std::string&, anh::ByteBuffer&)> AttributeFunctor;
			typedef std::map<std::string, AttributeFunctor> AttributeHandlerIndex;
			typedef std::map<std::string, AttributeFunctor>::const_iterator AttributeHandlerIndexIterator;
			//End Typedefs

			/**
				@brief An IFFInterpreter for object iff files.
			*/
			class oiff_interpreter : public interpreter_interface
			{
			public:
				/**
					@brief Constructor for an object iff interpreter
				*/
				oiff_interpreter();
				
				/**
					@brief returns the InterpreterType associated with this Interpreter
				*/
				virtual InterpreterType getType() { return OIFF_INTERPRETER; }
				
				/**
					@brief interprets a IFF::FileNode associated with this interpreter.
					This should only be called by the IFFFile code.
				*/
				virtual void interpret(std::shared_ptr<iff::file_node> node);

				/**
					@brief An internal ClientString structure. This could later be moved outside this class.
					It is merely here now for completeness.
				*/
				static class ClientString
				{
				public:
					std::string file;
					std::string entry;
				};

				/**
					@brief returns an attribute loaded from the object iff associated with this interpreter.

					@param key the key to lookup

					@return a boost::any casted shared_ptr<T> that was requested
				*/
				template <class T> std::shared_ptr<T> attribute(std::string& key);
				
				std::uint32_t attribute_uint32(std::string& key);
				float attribute_float(std::string& key);
				bool attribute_bool(std::string& key);


				/**
					@brief A function which causes this OIFFInterpreter to load parent files it needs and then copy
					their values downstream

					@param f the filemanager instance for retriving the necessary files.
				*/
				void load_aggregate_data(file_manager& f);

				/**
					@brief Causes this OIFFInterpreter to load any referenced files it has. It will use the filemanager
					to accomplish this.

					@param f the filemanager instance for retrieving the necessary files.
				*/
				void load_referenced_files(file_manager& f);

				/**
					@brief a simple output function for debugging values.
				*/
				void debug();

			private:
				//Internal Index used to link the handlers with the attributes
				static AttributeHandlerIndex attributeHandler_;

				//Internal Datatype handling functions used to parse the object files
				static void _handleUnhandled(oiff_interpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleClientString(oiff_interpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleString(oiff_interpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleInt(oiff_interpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleFloat(oiff_interpreter* dst, std::string& name, anh::ByteBuffer& buf);
				static void _handleBool(oiff_interpreter* dst, std::string& name, anh::ByteBuffer& buf);

				//Internal Node Handling functions
				void _handleXXXX(anh::ByteBuffer& buf);
				void _handleDERVXXXX(anh::ByteBuffer& buf);

				//Attributes this object iff might have
				std::map<std::string, boost::any> attributes_;
				
				//Parent files this object iff might have
				std::set<std::string> parentFiles;

				//Stored for determing if we've already loaded our aggregate information.
				bool has_aggregate_;
			};
		};
	};
};

#include "oiff_interpreter-intl.h"

#endif