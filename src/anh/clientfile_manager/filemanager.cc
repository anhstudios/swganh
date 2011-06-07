/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "filemanager.h"
#include "iff/interpreters/interpreter_interface.h"
#include "iff/iff.h"
#include "tre/tree.h"
#include "notfound_exception.h"

#include <iostream>
#include <fstream>
#include <regex>

using namespace anh::clientfile_manager;

file_manager::file_manager(std::string basedir)
	: basedir_(boost::filesystem::path(basedir))
{
	loadSWGTREConfig_();
}

file_manager::file_manager(boost::filesystem::path& basedir)
	: basedir_(basedir)
{
	loadSWGTREConfig_();
}

void file_manager::loadSWGTREConfig_()
{
	if(!exists(basedir_)) {
		throw NotFoundException("The directory specified: \"" + basedir_.file_string() + "\" could not found be found");
	}

	boost::filesystem::path searchDir((basedir_/"swg2uu_live.cfg"));

	std::vector<boost::filesystem::path> paths;
	std::ifstream config(searchDir.file_string());

	if(boost::filesystem::exists(searchDir))
	{
		std::string buffer;
		std::tr1::regex pattern("searchTree_.*= *\"(.+)\"");
		while(config.good())
		{
			std::getline(config, buffer);	
			std::tr1::smatch result;
			std::tr1::regex_search(buffer, result, pattern);
			if(result.size() > 0)
			{
				boost::filesystem::path path1(result[1].str());

				//We first assume they gave us an absolute path
				if(boost::filesystem::exists(path1))
				{
					paths.push_back(path1);
				}
				else
				{
					//It must be relative, so apply the basedir_
					paths.push_back(basedir_ / path1);
				}
			}
			buffer.clear();
		}

		for(int i=paths.size()-1; i >= 0; --i)
		{
			std::shared_ptr<tree::tree_archive> tree = std::make_shared<tree::tree_archive>(paths[i]);
			tree->start(fileMap_);
		}
	}
}

std::string file_manager::getSWGPath_(const std::string& base_string, const std::string& current_file)
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

bool file_manager::beginsWith_(const std::string& partial_path, const std::string& test_string)
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

bool file_manager::beginsWithNoDir_(const std::string& partial_path, const std::string& test_string)
{
	size_t partial_path_size = partial_path.size();
	size_t test_string_size = test_string.size();

	if(partial_path_size > test_string_size) 
		return false;

	for(size_t i = 0; i < test_string_size; ++i)
	{
		if(i < partial_path_size && partial_path[i] != test_string[i])
		{
			return false;
		} 
		else if(i >= partial_path_size && test_string[i] == '/')
		{
			return false;
		}
	}
	return true;
}