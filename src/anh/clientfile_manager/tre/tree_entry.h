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

#ifndef TREE_ENTRY_H
#define TREE_ENTRY_H

#include <cstdint> //For the std::uint32_t's

namespace anh
{
	namespace clientfile_manager
	{
		namespace tree
		{
			//Forward Declarations
			class tree_archive;

			//End Forward Declarations

			/**
				@brief An object used to index .tre files.
				Should not appear outside the TreeArchive and FileManager classes.
			*/
			class tree_entry
			{
			public:
				std::uint32_t checksum() {return checksum_;}
				std::uint32_t uncompressed_size() {return uncompressed_size_;}
				std::uint32_t fileOffset() {return fileOffset_;}
				std::uint32_t compressionType() {return compressionType_;}
				std::uint32_t compressed_size() {return compressed_size_;}
				tree_archive* parentTree() {return parentTree_;}
			private:

				std::uint32_t checksum_;
				std::uint32_t uncompressed_size_;
				std::uint32_t fileOffset_;
				std::uint32_t compressionType_;
				std::uint32_t compressed_size_;
				std::uint32_t name_offset_;

				tree_archive* parentTree_;

				friend tree_archive; //We only want tree_archives making tree_entries.
				tree_entry() {}
			};
		};
	};
};
#endif