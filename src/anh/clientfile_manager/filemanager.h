#ifndef CLIENT_FILE_MANAGER_H
#define CLIENT_FILE_MANAGER_H

#include "tre/tree_entry.h" //Included to satisfy the templates

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
				@param postBuildProcess a functor which specifies something to be done to each IFFImeplementation after it is loaded.
			
				@return a vector of shared pointers to the IFFImplementation requested.
			*/
			template <class T> std::vector<std::shared_ptr<T>> load_directory(const std::string& directory, const std::string& ext="", std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess=nullptr);
		
			/**

				@brief Recursively Loads a directory structure of swg files from Memory(already loaded), filesystem, or a .tre file whichever comes first.

				@param directory the swg directory. This is _not necessarily_ a valid filesystem filename.
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