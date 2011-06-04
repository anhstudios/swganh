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

#ifndef TREE_H
#define TREE_H

#include <string> //For String
#include <map> //For Map
#include <boost/filesystem.hpp> //For Boost FileSystem Path

//Begin Forward Declarations
namespace anh
{
	class ByteBuffer;
};

namespace ClientFileManager
{
	namespace TRE
	{
		struct TreeEntry;
		//End Forward Declarations

		/**
			@brief Acts as a frontend for a .tre file providing convienience methods like getFile()
		*/
		class TreeArchive
		{
		public:
			/**
				@brief Constructs a TreeArchive File.

				@param filename a a boost filesystem path of the filesystem path of the .tre file to open.
				@param entries a reference a map into which this archive will insert it's index.
			*/
			TreeArchive(boost::filesystem::path& path, std::map<std::string, TRE::TreeEntry>& entries)
			{
				intrl_stream = fopen(path.file_string().c_str(), "rb");
				_loadTree(intrl_stream, entries);
			}

			/**
				@brief Constructs a TreeArchive File.

				@param filename a string of the filesystem path of the .tre file to open.
				@param entries a reference a map into which this archive will insert it's index.
			*/
			TreeArchive(std::string& filename, std::map<std::string, TRE::TreeEntry>& entries)
			{	
				intrl_stream = fopen(filename.c_str(), "rb");
				_loadTree(intrl_stream, entries);
			}

			/**
				@brief Default destructor for the TreeArchive.

				Closes the internal stream associated with this Tree
			*/
			~TreeArchive()
			{
				fclose(intrl_stream);
			}

			/**
				@ brief Reads a file from this .tre file.

				@param entry the look up entry to use to read the file.

				@return An anh::ByteBuffer of the data of the swg file requested.
			*/
			anh::ByteBuffer getFile(const TRE::TreeEntry& entry);

		private:
			TreeArchive();
			TreeArchive(const TreeArchive& t);

			//Internal Constructor (called by all actual constructors)
			void _loadTree(FILE* inputstream, std::map<std::string, TRE::TreeEntry>& entries);

			//An open stream object to the .tre file.
			FILE* intrl_stream;
		};
	};
};

#endif