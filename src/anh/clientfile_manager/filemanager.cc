/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team */

#include "filemanager.h"
#include "interpreters/interpreter_interface.h"
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
	//We first confirm that the base directory exists
	if(!exists(basedir_)) {
		//It doesn't, so we throw this exception in accordance with the specifications
		throw NotFoundException("The directory specified: \"" + basedir_.file_string() + "\" could not found be found");
	}

	//Okay, the base directory is OK, so we initialize some variables
	boost::filesystem::path searchDir((basedir_/"swg2uu_live.cfg"));
	std::vector<boost::filesystem::path> paths;
	std::ifstream config(searchDir.file_string());
	
	//If the tree file aggregate index exists, we load it
	if(boost::filesystem::exists(searchDir))
	{
		//Initialize some more variables used in the loading process
		std::string buffer;
		std::tr1::regex pattern("searchTree_.*= *\"(.+)\""); //Including this nifty regex pattern
		std::tr1::smatch result;
		while(config.good()) //While the file still has more to give us
		{
			//Read a line
			std::getline(config, buffer);	
			std::tr1::regex_search(buffer, result, pattern); //Break it apart with regex
			if(result.size() > 1) //The result must be bigger than the entire string we gave regex
			{
				//So, We first assume they gave us an absolute path
				boost::filesystem::path path1(result[1].str());
				if(boost::filesystem::exists(path1))
				{
					//It was, so put it into the listing
					paths.push_back(path1);
				}
				else
				{
					path1 = basedir_ / path1;
					if(boost::filesystem::exists(path1))
					{
						paths.push_back(basedir_ / path1);
					}
				}
			}
		}

		//For every path we have, create the tree archive and then load it's data
		for(int i=paths.size()-1; i >= 0; --i)
		{
			//Note that the tree archives are not held in a container. This is OK because the shared_ptr to the
			//tree archive is held by the entries in the fileMap_. This allows the tree_archive to automatically be
			//destroyed when the entries are removed from the fileMap and destroyed.
			std::shared_ptr<tree::tree_archive> tree = std::make_shared<tree::tree_archive>(paths[i]);
			tree->start(fileMap_);
		}
	}
}

std::string file_manager::getSWGPath_(const std::string& base_string, const std::string& current_file)
{
	//Note: We can portably use the '/' character because this function should be called with boost::filesystem::path.string()
	//instead of file_string(). This causes us to get a filesystem internal representation that uses '/'
	if(base_string.size() > 0 && base_string[base_string.size()-1] == '/')
	{
		//The base string should always be smaller than the current file for protection
		if(current_file.size() > base_string.size())
		{
			//We just get the run of the mill intersection
			return current_file.substr(base_string.size(), current_file.size());
		}
	}
	else
	{
		//Again, we need to make sure we're big enough
		if(current_file.size() > base_string.size()+1)
		{
			//We get the run of the mill intersection, BUT we need to chop off that first slash!
			return current_file.substr(base_string.size()+1, current_file.size());
		}
	}
	
	//We ran into a problem, so lets return an empty string
	return "";
}

bool file_manager::beginsWith_(const std::string& partial_path, const std::string& test_string)
{
	//If the sizing isn't right, then we definately don't begin with the partial_path!
	if(partial_path.size() > test_string.size()) 
		return false;

	//Go through and confirm that for every char of partial_path, test_string is equal
	for(size_t i = 0; i < partial_path.size(); ++i)
	{
		if(partial_path[i] != test_string[i])
		{
			//It's not!
			return false;
		}
	}

	//It was!
	return true;
}

bool file_manager::beginsWithNoDir_(const std::string& partial_path, const std::string& test_string)
{
	//Get the sizes real quick so we save some useless calls
	size_t partial_path_size = partial_path.size();
	size_t test_string_size = test_string.size();

	//If the sizing isn't right, then we definately don't begin with the partial_path!
	if(partial_path_size > test_string_size) 
		return false;

	//We need to go through the ENTIRE test stirng to make sure we don't have any extra directories in there
	for(size_t i = 0; i < test_string_size; ++i)
	{
		//If we're still within range of partial path, confirm equivalency
		if(i < partial_path_size && partial_path[i] != test_string[i])
		{
			return false; //It's not!
		} 
		else if(i >= partial_path_size && test_string[i] == '/') //otherwise we check for slashes
		{
			return false; //We found one!
		}
	}
	return true; //Looks good!
}

bool file_manager::endsWith_(const std::string& end, const std::string& test_string)
{
	//Get the sizes real quick so we save some useless calls
	size_t end_size = end.size();
	size_t test_string_size = test_string.size();

	//If the sizes arn't right, lets not even waste our time
	if(end_size > test_string_size)
		return false;

	//For ever character in end size, we need to confirm equivalency
	for(size_t i=1; i <= end_size; ++i)
	{
		if(end[end_size-i] != test_string[test_string_size-i])
		{
			return false; //We found something that wasn't right!
		}
	}
	return true;//Looks good
}