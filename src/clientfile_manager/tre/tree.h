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

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <memory>

namespace anh
{
	class ByteBuffer;
};

namespace ClientFileManager
{
	namespace TRE
	{
		struct TreeEntry;

		class TreeArchive
		{
		public:
			TreeArchive(boost::filesystem::path& path, std::vector<TreeEntry>& entries)
				: filename_(path.file_string())
			{
				FILE* stream = fopen(path.file_string().c_str(), "rb");
				_loadTree(stream, entries);
				fclose(stream);
			}

			TreeArchive(std::string& filename, std::vector<TreeEntry>& entries)
				: filename_(filename)
			{	
				FILE* stream = fopen(filename.c_str(), "rb");
				_loadTree(stream, entries);
				fclose(stream);
			}

			anh::ByteBuffer getFile(TreeEntry& entry);

		private:
			std::uint32_t record_count;
			std::uint32_t lookupData_offset;
			std::uint32_t lookupData_compression;
			std::uint32_t lookupData_currentSize;
			std::uint32_t lookupData_finalSize; //CALCULATED NOT READ
			std::uint32_t names_offset; //CALCUATED NOT READ
			std::uint32_t names_compression;
			std::uint32_t names_currentSize;
			std::uint32_t names_finalSize;

			void _loadTree(FILE* inputstream, std::vector<TreeEntry>& entries);
			std::string filename_;
		};
	};
};

#endif