#ifndef CLIENT_FILE_MANAGER_H
#define CLIENT_FILE_MANAGER_H

#include <string>
#include <boost/filesystem.hpp>
#include <boost/flyweight.hpp>
#include <vector>
#include <map>
#include <memory>

#include "tre/tree_entry.h"

namespace ClientFileManager
{
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

	typedef std::map<boost::flyweight<std::string>, IFF::IFFFile> LoadedMap;
	typedef std::map<boost::flyweight<std::string>, IFF::IFFFile>::iterator LoadedMapIterator;

	typedef std::map<boost::flyweight<std::string>, TRE::TreeEntry> FileMap;
	typedef std::map<boost::flyweight<std::string>, TRE::TreeEntry>::iterator FileMapIterator;

	class FileManager
	{
	public:
		FileManager(std::string basedir);
		FileManager(boost::filesystem::path& basedir);

		IFF::IFFFile getFile(const std::string& filename, std::shared_ptr<IFF::Interpreters::IFFInterpreter> interpreter);
	private:
		void _loadSWGTREConfig();

		boost::filesystem::path basedir_;
		std::vector<TRE::TreeArchive*> trees;
		LoadedMap loadedFileMap_;
		FileMap fileMap_;
	};
};
#endif