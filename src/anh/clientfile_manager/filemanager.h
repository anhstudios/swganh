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

#ifndef CLIENT_FILE_MANAGER_H
#define CLIENT_FILE_MANAGER_H

#include <anh/clientfile_manager/tre/tree_entry.h> //Included to satisfy the templates
#include <boost/any.hpp> //Included for boost::any
#include <boost/filesystem.hpp> //Included for boost::filesystem::path
#include <string> //Included for String
#include <map> //Included for Map
#include <memory> //Included for shared_ptr

namespace anh
{
	namespace clientfile_manager
	{
		//Begin Forward Declarations
		namespace iff
		{
			class iff_file;
			
		};

		namespace interpreters
		{
			class interpreter_interface;
		};

		namespace tree
		{
			class tree_archive;
		};
		//End Forward Declarations

		//Begin Typedefs
		typedef std::map<std::string, const tree::tree_entry> FileMap;
		typedef std::map<std::string, boost::any> LoadedMap;
		typedef std::map<std::string, boost::any>::const_iterator LoadedMapIterator;
		typedef std::map<std::string, const tree::tree_entry>::iterator FileMapIterator;
		//End Typedefs

		/**
			@brief This is the backbone of the clientfile manager. It loads files and entire directories of swg files.

			To do this, it manages an interal index of the swg .tre files. In addition, it also keeps track of files that
			have already been loaded so that no file is ever reloaded.
		*/
		class file_manager
		{
		public:

			/**
				@brief Constructs a FileManager using the file system directory provided.

				@param basedir the a string to the filesystem directory to treat as the base directory.
			*/
			file_manager(std::string basedir);
		
			/**
				@brief Constructs a FileManager using the file system directory provided.

				@param basedir the a a boost filesystem path object to the filesystem directory to treat as the base directory.
			*/
			file_manager(boost::filesystem::path& basedir);

			/**

				@brief Loads a single swg file from Memory(already loaded), the filesystem or .tre file, which ever comes first. 

				@param filename a swg filename. This is _not necessarily_ a valid filesystem filename.
				@param postBuildProcess a functor which specifies something to be done to each IFFImplementation after it is loaded.

				@return a shared pointer to the IFFImplementation requested.

			*/
			template <class T> std::shared_ptr<T> load(const std::string& filename, std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess=nullptr);
		
			/**

				@brief Loads a single directory's swg files from Memory(already loaded), filesystem, or a .tre file whichever comes first.

				@param directory the swg directory. This is _not necessarily_ a valid filesystem filename.
				@param ext an optional argument for a file extension to limit the files loaded from the directory to.
				@param postBuildProcess a functor which specifies something to be done to each IFFImeplementation after it is loaded.
			
				@return a vector of shared pointers to the IFFImplementation requested.
			*/
			template <class T> std::vector<std::shared_ptr<T>> load_directory(const std::string& directory, const std::string& ext="", std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess=nullptr);
		
			/**

				@brief Recursively Loads a directory structure of swg files from Memory(already loaded), filesystem, or a .tre file whichever comes first.

				@param directory the swg directory. This is _not necessarily_ a valid filesystem filename.
				@param ext an optional argument for a file extension to limit the files loaded from the directories to.
				@param postBuildProcess a functor which specifies something to be done to each IFFImeplementation after it is loaded.
			
				@return a vector of shared pointers to the IFFImplementation requested.
			*/
			template <class T> std::vector<std::shared_ptr<T>> load_all(const std::string& directory, const std::string& ext="", std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess=nullptr);

		private:
			file_manager(); //Default Constructor
			file_manager(const file_manager& f); //copy constructor

			void loadSWGTREConfig_(); //Internal Constructor (called by all actual constructors)

			//Utility Functions
			bool beginsWith_(const std::string& partial_path, const std::string& test_string);
			bool beginsWithNoDir_(const std::string& partial_path, const std::string& test_string);
			bool endsWith_(const std::string& end, const std::string& test_string);

			std::string getSWGPath_(const std::string& base_string, const std::string& current_file);
			//End Utility Functions

			boost::filesystem::path basedir_; //The base directory where the swg2uu_live.cfg is found along with .tre overrides.
		
			LoadedMap loadedFileMap_; //Loaded .iff files have their Implementations stored here for safe keeping.

			FileMap fileMap_; //Tre Index, used to find the appropriate .tre file and place for extraction.
		};
	};
};
#include "filemanager-inl.h" //Implements the templates

#endif