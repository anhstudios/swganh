#include <iostream>
#include <fstream>

#include <regex>

#include "filemanager.h"
#include "iff/iff.h"
#include "tre/tree.h"
#include "tre/tree_entry.h"

using namespace ClientFileManager;

void FileManager::_loadSWGTREConfig()
{
	std::vector<boost::filesystem::path> paths;
	std::ifstream config((basedir_/"swg2uu_live.cfg").file_string());

	std::string buffer;
	std::tr1::regex pattern("searchTree_.*= *\"(.+)\"");
	while(config.good())
	{
		std::getline(config, buffer);	
		std::tr1::smatch result;
		std::tr1::regex_search(buffer, result, pattern);
		if(result.size() > 0)
		{
			paths.push_back(boost::filesystem::path(result[1]));
		}
		buffer.clear();
	}

	for(int i=paths.size()-1; i >= 0; --i)
	{
		std::vector<std::shared_ptr<TRE::TreeEntry>> entries;
		TRE::TreeArchive* tre = new TRE::TreeArchive(paths[i], entries);
		for(unsigned int j = 0; j < entries.size(); ++j)
		{
			fileMap_.insert(std::make_pair<std::string, std::shared_ptr<TRE::TreeEntry>>(entries[j]->nameString, entries[j]));
		}
	}
}

IFF::IFFFile* FileManager::getFile(const std::string& filename, IFF::Interpreters::IFFInterpreter* interpreter)
{
	LoadedMapIterator lIt = loadedFileMap_.find(filename);

	if(lIt != loadedFileMap_.end())
	{
		std::cout << "Already Loaded. Serving." << std::endl;

		//We found it!
		return lIt->second;
	}
	else
	{
		//First we check the base directory
		boost::filesystem::path p(basedir_ / filename);

		if(boost::filesystem::is_regular_file(p))
		{
			std::cout << "Found in the Filesystem. Loading from file." << std::endl;

			FILE* f = fopen(p.file_string().c_str(), "rb");
			return new IFF::IFFFile(f, interpreter);
		}
		else
		{
			//Then we check the map.
			FileMapIterator fIt = fileMap_.find(filename); //That's right. F IT

			if(fIt != fileMap_.end())
			{
				std::cout << "Found in a tre. Loading from there." << std::endl;

				std::shared_ptr<TRE::TreeEntry> entry = fIt->second;
				return new IFF::IFFFile(*entry->parentTree->getFile(entry.get()), interpreter);
			}
			else
			{
				return nullptr; //Last resort
			}
		}
	}
}

FileManager::FileManager(std::string basedir)
	: basedir_(boost::filesystem::path(basedir))
{
	_loadSWGTREConfig();
}

FileManager::FileManager(boost::filesystem::path& basedir)
	: basedir_(basedir)
{
	_loadSWGTREConfig();
}