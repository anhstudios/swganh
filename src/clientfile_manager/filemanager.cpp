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

std::string FileManager::_getSWGPath(const std::string& base_string, const std::string& current_file)
{
	if(base_string.size() > 0 && base_string[base_string.size()-1] == '/')
	{
		if(current_file.size() > base_string.size())
		{
			return current_file.substr(base_string.size(), current_file.size());
		}
	}
	else
	{
		if(current_file.size() > base_string.size()+1)
		{
			return current_file.substr(base_string.size()+1, current_file.size());
		}
	}
	return "";
}

bool FileManager::_beginsWith(const std::string& partial_path, const std::string& test_string)
{
	if(partial_path.size() > test_string.size()) 
		return false;

	for(size_t i = 0; i < partial_path.size(); ++i)
	{
		if(partial_path[i] != test_string[i])
		{
			return false;
		}
	}
	return true;
}

bool FileManager::_beginsWithNoDir(const std::string& partial_path, const std::string& test_string)
{
	if(partial_path.size() > test_string.size()) 
		return false;

	for(size_t i = 0; i < test_string.size(); ++i)
	{
		if(i < partial_path.size() && partial_path[i] != test_string[i])
		{
			return false;
		} 
		else if(test_string[i] == '/')
		{
			return false;
		}
	}
	return true;
}

void FileManager::_loadSWGTREConfig()
{
	if(!exists(basedir_)) {return ;}
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
		TRE::TreeArchive* tre = new TRE::TreeArchive(paths[i], fileMap_);
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