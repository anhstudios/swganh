/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include <iostream>
#include <fstream>

#include <regex>

#include "filemanager.h"
#include "iff/interpreters/IFFInterpreter.h"
#include "iff/iff.h"
#include "tre/tree.h"

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
		std::vector<TRE::TreeEntry> entries;
		TRE::TreeArchive* tre = new TRE::TreeArchive(paths[i], entries);
		for(unsigned int j = 0; j < entries.size(); ++j)
		{
			fileMap_.insert(std::make_pair<std::string, TRE::TreeEntry>(entries[j].nameString, std::move(entries[j])));
		}
	}
}

bool FileManager::hasFile(const std::string& filename)
{
	boost::flyweight<std::string> weight(filename);
	LoadedMapIterator lIt = loadedFileMap_.find(weight);
	if(lIt != loadedFileMap_.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::shared_ptr<IFF::Interpreters::IFFInterpreter> FileManager::getFile(const std::string& filename)
{
	boost::flyweight<std::string> weight(filename);
	LoadedMapIterator lIt = loadedFileMap_.find(weight);
	if(lIt != loadedFileMap_.end())
	{
		std::cout << "Already Loaded. Serving." << std::endl;
		//We found it!
		return lIt->second;
	}
	else
	{
		return nullptr;
	}

}

std::shared_ptr<IFF::Interpreters::IFFInterpreter> FileManager::getFile(const std::string& filename, std::shared_ptr<IFF::Interpreters::IFFInterpreter> interpreter)
{
	boost::flyweight<std::string> weight(filename);

	LoadedMapIterator lIt = loadedFileMap_.find(weight);
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
			IFF::IFFFile iff(f, interpreter);

			return interpreter;
		}
		else
		{
			//Then we check the map.
			FileMapIterator fIt = fileMap_.find(weight); //That's right. F IT

			if(fIt != fileMap_.end())
			{
				std::cout << "Found in a tre. Loading from there." << std::endl;

				TRE::TreeEntry entry = std::move(fIt->second);
				IFF::IFFFile iff(entry.parentTree->getFile(entry), interpreter);

				return interpreter;
			}
			else
			{
				return nullptr;
				//throw new IFF::NotFoundException("Couldn't find the file in the trees. No word yet about the bushes.");
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