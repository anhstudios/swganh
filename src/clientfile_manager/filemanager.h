#ifndef CLIENT_FILE_MANAGER_H
#define CLIENT_FILE_MANAGER_H

#include <boost/any.hpp> //Included for boost::any
#include <boost/filesystem.hpp> //Included for boost::filesystem::path
#include <string> //Included for String
#include <map> //Included for Map
#include <memory> //Included for shared_ptr

#include "tre/tree_entry.h" //Included to satisfy the templates

namespace ClientFileManager
{
	//Begin Forward Declarations
	namespace IFF
	{
		class IFFFile;
		namespace Interpreters
		{
			class IFFInterpreter;
		};
	};

	namespace TRE
	{
		class TreeArchive;
		struct TreeEntry;
	};
	//End Forward Declarations

	//Begin Typedefs
	typedef std::map<std::string, TRE::TreeEntry> FileMap;
	typedef std::map<std::string, boost::any> LoadedMap;
	typedef std::map<std::string, boost::any>::const_iterator LoadedMapIterator;
	typedef std::map<std::string, TRE::TreeEntry>::const_iterator FileMapIterator;
	//End Typedefs

	/**
		@brief This is the backbone of the clientfile manager. It loads files and entire directories of swg files.

		To do this, it manages an interal index of the swg .tre files. In addition, it also keeps track of files that
		have already been loaded so that no file is ever reloaded.
	*/
	class FileManager
	{
	public:

		/**
			@brief Constructs a FileManager using the file system directory provided.

			@param basedir the a string to the filesystem directory to treat as the base directory.
		*/
		FileManager(std::string basedir);
		
		/**
			@brief Constructs a FileManager using the file system directory provided.

			@param basedir the a a boost filesystem path object to the filesystem directory to treat as the base directory.
		*/
		FileManager(boost::filesystem::path& basedir);

		/**

			@brief Loads a single swg file from Memory(already loaded), the filesystem or .tre file, which ever comes first. 

			@param filename a swg filename. This is _not necessarily_ a valid filesystem filename.
			@param postBuildProcess a functor which specifies something to be done to each IFFImplementation after it is loaded.

			@return a shared pointer to the IFFImplementation requested.

		*/
		template <class T> std::shared_ptr<T> load(const std::string& filename, std::function<void(std::shared_ptr<T>, FileManager&)> postBuildProcess=nullptr);
		
		/**

			@brief Loads a single directory's swg files from Memory(already loaded), filesystem, or a .tre file whichever comes first.

			@param directory the swg directory. This is _not necessarily_ a valid filesystem filename.
			@param postBuildProcess a functor which specifies something to be done to each IFFImeplementation after it is loaded.
			
			@return a vector of shared pointers to the IFFImplementation requested.
		*/
		template <class T> std::vector<std::shared_ptr<T>> loadDirectory(const std::string& directory, std::function<void(std::shared_ptr<T>, FileManager&)> postBuildProcess=nullptr);
		
		/**

			@brief Recursively Loads a directory structure of swg files from Memory(already loaded), filesystem, or a .tre file whichever comes first.

			@param directory the swg directory. This is _not necessarily_ a valid filesystem filename.
			@param postBuildProcess a functor which specifies something to be done to each IFFImeplementation after it is loaded.
			
			@return a vector of shared pointers to the IFFImplementation requested.
		*/
		template <class T> std::vector<std::shared_ptr<T>> loadAll(const std::string& directory, std::function<void(std::shared_ptr<T>, FileManager&)> postBuildProcess=nullptr);

	private:
		FileManager(); //Default Constructor
		FileManager(const FileManager& f); //copy constructor

		void _loadSWGTREConfig(); //Internal Constructor (called by all actual constructors)

		//Utility Functions
		bool _beginsWith(const std::string& partial_path, const std::string& test_string);
		bool _beginsWithNoDir(const std::string& partial_path, const std::string& test_string);
		std::string _getSWGPath(const std::string& base_string, const std::string& current_file);
		//End Utility Functions

		boost::filesystem::path basedir_; //The base directory where the swg2uu_live.cfg is found along with .tre overrides.
		
		LoadedMap loadedFileMap_; //Loaded .iff files have their Implementations stored here for safe keeping.

		FileMap fileMap_; //Tre Index, used to find the appropriate .tre file and place for extraction.
	};
};

#include "filemanager-inl.h" //Implements the templates

#endif