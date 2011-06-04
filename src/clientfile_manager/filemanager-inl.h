
#include "tre/tree.h"
#include <set>

namespace ClientFileManager
{
	template <class T> std::shared_ptr<T> FileManager::load(const std::string& filename, std::function<void(std::shared_ptr<T>, FileManager&)> postBuildProcess)
	{
		//We will check for the file requested in the following order:
		//1. Already Loaded
		//2. On Hard Disk
		//3. In a .tre File

		boost::filesystem::path p(filename);
		LoadedMapIterator lIt = loadedFileMap_.find(filename);
		if(lIt != loadedFileMap_.end())
		{
			return boost::any_cast<std::shared_ptr<T>>(lIt->second);
		}
		else
		{
			boost::filesystem::path p2 = basedir_ / filename;
			if(boost::filesystem::is_regular_file(p2))
			{
				std::shared_ptr<T> t = std::make_shared<T>();
				IFF::IFFFile iff(p2.file_string(), t);
				loadedFileMap_.insert(std::make_pair<std::string, boost::any>(filename, t));
				if(postBuildProcess != nullptr)
				{
					postBuildProcess(t, *this);
				}
				return t;
			}
			else
			{
				FileMapIterator fIt = fileMap_.find(filename);
				if(fIt != fileMap_.end())
				{
					std::shared_ptr<T> t = std::make_shared<T>();
					IFF::IFFFile iff(fIt->second.parentTree->getFile(fIt->second), t);
					loadedFileMap_.insert(std::make_pair<std::string, boost::any>(filename, t));
					if(postBuildProcess != nullptr)
					{
						postBuildProcess(t, *this);
					}
					return t;
				}
			}
		}
		return nullptr;
	}

	template <class T> std::vector<std::shared_ptr<T>> FileManager::loadDirectory(const std::string& directory, std::function<void(std::shared_ptr<T>, FileManager&)> postBuildProcess)
	{
		std::vector<std::shared_ptr<T>> result;
		std::set<std::string> previously_loaded;

		//Stage 1 = Load the files on the disk
		boost::filesystem::directory_iterator end_itr;
		for(boost::filesystem::directory_iterator itr(basedir_ / directory); itr != end_itr; ++itr)
		{
			if(boost::filesystem::is_regular_file(itr->status()))
			{
				std::string swg_name = _getSWGPath(basedir_.string(), itr->string());
				auto lfmi = loadedFileMap_.find(swg_name);
				if(lfmi == loadedFileMap_.end())
				{
					std::shared_ptr<T> t = std::make_shared<T>();
					IFF::IFFFile iff(itr->path().file_string(), t);
					loadedFileMap_.insert(std::make_pair<std::string, boost::any>(swg_name, t));
					if(postBuildProcess != nullptr)
					{
						postBuildProcess(t, *this);
					}
					result.push_back(t);
				}
				else
				{
					result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
				}
				previously_loaded.insert(std::move(swg_name));
			}
		}

		bool found_chunk = false;
		//Stage 2 = Check the files in the trees
		FileMapIterator it = fileMap_.begin();
		FileMapIterator end = fileMap_.end();
		while(it != end)
		{
			if(_beginsWithNoDir(directory, it->first))
			{
				found_chunk = true;
				if(previously_loaded.find(it->first) == previously_loaded.end())
				{
					//Load it
					boost::filesystem::path p(basedir_ / it->first);
					std::shared_ptr<T> t = std::make_shared<T>();
					IFF::IFFFile iff(p.file_string(), t);
					loadedFileMap_.insert(std::make_pair<std::string, boost::any>(it->first, t));
					if(postBuildProcess != nullptr)
					{
						postBuildProcess(t, *this);
					}
					result.push_back(t);
				}
			}
			else if(found_chunk)
			{
				//The map is sorted, so we've passed our chunk.
				break; 
			}
			++it;
		}
		return result;
	}

	template <class T> std::vector<std::shared_ptr<T>> FileManager::loadAll(const std::string& directory, std::function<void(std::shared_ptr<T>, FileManager&)> postBuildProcess)
	{
		std::vector<std::shared_ptr<T>> result;
		std::set<std::string> previously_loaded;

		//Stage 1 = Load the files on the disk
		boost::filesystem::recursive_directory_iterator end_itr;
		for(boost::filesystem::recursive_directory_iterator itr(basedir_ / directory); itr != end_itr; ++itr)
		{
			if(boost::filesystem::is_regular_file(itr->status()))
			{
				std::string swg_name = _getSWGPath(basedir_.string(), itr->string());
				auto lfmi = loadedFileMap_.find(swg_name);
				if(lfmi == loadedFileMap_.end())
				{
					std::shared_ptr<T> t = std::make_shared<T>();
					IFF::IFFFile iff(itr->path().file_string(), t);
					loadedFileMap_.insert(std::make_pair<std::string, boost::any>(swg_name, t));
					if(postBuildProcess != nullptr)
					{
						postBuildProcess(t, *this);
					}
					result.push_back(t);
				}
				else
				{
					result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
				}
				previously_loaded.insert(std::move(swg_name));
			}
		}

		bool found_chunk = false;
		//Stage 2 = Check the files in the trees
		FileMapIterator it = fileMap_.begin();
		FileMapIterator end = fileMap_.end();
		while(it != end)
		{
			if(_beginsWith(directory, it->first))
			{
				found_chunk = true;
				if(previously_loaded.find(it->first) == previously_loaded.end())
				{
					//Load it
					boost::filesystem::path p(basedir_ / it->first);
					std::shared_ptr<T> t = std::make_shared<T>();
					IFF::IFFFile iff(p.file_string(), t);
					loadedFileMap_.insert(std::make_pair<std::string, boost::any>(it->first, t));
					if(postBuildProcess != nullptr)
					{
						postBuildProcess(t, *this);
					}
					result.push_back(t);
					//previously_loaded.insert(it->first.c_str());
				}
			}
			else if(found_chunk)
			{
				//The map is sorted, so we've passed our chunk.
				break; 
			}
			++it;
		}
		return result;
	}
};