#ifndef FILE_MANAGER_INL_H
#define FILE_MANAGER_INL_H

#include "tre/tree.h"
#include "iff/iff.h"
#include <set>

namespace anh
{
	namespace clientfile_manager
	{
		template <class T> std::shared_ptr<T> file_manager::load(const std::string& filename, std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess)
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
					iff::iff_file iff(p2.file_string(), t);
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
						
						auto temp_object = fIt->second;

						iff::iff_file iff(temp_object.parentTree()->getFile(temp_object), t);
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

		template <class T> std::vector<std::shared_ptr<T>> file_manager::load_directory(const std::string& directory, const std::string& ext, std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess)
		{
			std::vector<std::shared_ptr<T>> result;
			std::set<std::string> previously_loaded;

			boost::filesystem2::path searchDir(basedir_ / directory);
			if(boost::filesystem::exists(searchDir))
			{
				//Stage 1 = Load the files on the disk
				boost::filesystem::directory_iterator end_itr;
				for(boost::filesystem::directory_iterator itr(searchDir); itr != end_itr; ++itr)
				{
					if(boost::filesystem::is_regular_file(itr->status()))
					{
						std::string swg_name = getSWGPath_(basedir_.string(), itr->string());

						if(!endsWith_(ext, swg_name))
							continue;

						auto lfmi = loadedFileMap_.find(swg_name);
						if(lfmi == loadedFileMap_.end())
						{
							std::shared_ptr<T> t = std::make_shared<T>();
							iff::iff_file iff(itr->path().file_string(), t);
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
			}

			bool found_chunk = false;
			//Stage 2 = Check the files in the trees
			FileMapIterator it = fileMap_.begin();
			FileMapIterator end = fileMap_.end();
			while(it != end)
			{
				if(beginsWithNoDir_(directory, it->first) && endsWith_(ext, it->first))
				{
					found_chunk = true;

					auto lfmi = loadedFileMap_.find(it->first);
					if(lfmi == loadedFileMap_.end())
					{
						if(previously_loaded.find(it->first) == previously_loaded.end())
						{
							//Load it
							boost::filesystem::path p(basedir_ / it->first);
							std::shared_ptr<T> t = std::make_shared<T>();

							auto temp_obj = it->second;
							iff::iff_file iff(temp_obj.parentTree()->getFile(temp_obj), t);
							loadedFileMap_.insert(std::make_pair<std::string, boost::any>(it->first, t));
							if(postBuildProcess != nullptr)
							{
								postBuildProcess(t, *this);
							}
							result.push_back(t);
						}
					}
					else
					{
						result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
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

		template <class T> std::vector<std::shared_ptr<T>> file_manager::load_all(const std::string& directory, const std::string& ext, std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess)
		{
			std::vector<std::shared_ptr<T>> result;
			std::set<std::string> previously_loaded;

			//Stage 1 = Load the files on the disk
			boost::filesystem2::path searchDir(basedir_ / directory);
			if(boost::filesystem::exists(searchDir))
			{
				boost::filesystem::recursive_directory_iterator end_itr;
				for(boost::filesystem::recursive_directory_iterator itr(searchDir); itr != end_itr; ++itr)
				{
					if(boost::filesystem::is_regular_file(itr->status()))
					{
						std::string swg_name = getSWGPath_(basedir_.string(), itr->string());

						if(!endsWith_(ext, swg_name))
							continue;

						auto lfmi = loadedFileMap_.find(swg_name);
						if(lfmi == loadedFileMap_.end())
						{
							std::shared_ptr<T> t = std::make_shared<T>();
							iff::iff_file iff(itr->path().file_string(), t);
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
			}

			bool found_chunk = false;
			//Stage 2 = Check the files in the trees
			FileMapIterator it = fileMap_.begin();
			FileMapIterator end = fileMap_.end();
			while(it != end)
			{
				if(beginsWith_(directory, it->first) && endsWith_(ext, it->first))
				{
					found_chunk = true;
					auto lfmi = loadedFileMap_.find(it->first);
					if(lfmi == loadedFileMap_.end())
					{
						if(previously_loaded.find(it->first) == previously_loaded.end())
						{
							//Load it
							boost::filesystem::path p(basedir_ / it->first);
							std::shared_ptr<T> t = std::make_shared<T>();

							auto temp_obj = it->second;
							iff::iff_file iff(temp_obj.parentTree()->getFile(temp_obj), t);
							loadedFileMap_.insert(std::make_pair<std::string, boost::any>(it->first, t));
							if(postBuildProcess != nullptr)
							{
								postBuildProcess(t, *this);
							}
							result.push_back(t);
							//previously_loaded.insert(it->first.c_str());
						}
					}
					else
					{
						result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
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
};
#endif