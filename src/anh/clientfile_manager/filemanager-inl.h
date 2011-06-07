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
			//First we check the loaded map
			LoadedMapIterator lIt = loadedFileMap_.find(filename);
			if(lIt != loadedFileMap_.end())
			{
				//We found it! Return it because it's already loaded.
				return boost::any_cast<std::shared_ptr<T>>(lIt->second);
			}
			else
			{
				//It wasn't loaded, so now we check the filesystem
				boost::filesystem::path p = basedir_ / filename;
				if(boost::filesystem::is_regular_file(p))
				{
					//We found it! Okay, so now we load it.
					std::shared_ptr<T> t = std::make_shared<T>(); //Make the interpreter we need
					
					//We don't actually need the iff_file for anything except loading, so
					//create it anonymously
					iff::iff_file(p.file_string(), t);

					//We're finished loading, so we put it into the loadedFileMap for safe keeping
					loadedFileMap_.insert(std::make_pair<std::string, boost::any>(filename, t));

					//And then we call the postBuild functor if we were given one.
					if(postBuildProcess != nullptr)
					{
						postBuildProcess(t, *this);
					}

					//Finally we return the loaded file
					return t;
				}
				else
				{
					//We didn't find it in memory on the filesystem, so we load from tree.

					//Look up the necessary tree_entry
					FileMapIterator fIt = fileMap_.find(filename);
					if(fIt != fileMap_.end())
					{
						//Good. We found it.
						std::shared_ptr<T> t = std::make_shared<T>(); //Create the necessary interpreter
						
						//The compiler gets pissy when we don't have this temp object here.
						auto temp_object = fIt->second;

						//Anonymously make the iff file since we won't actually use it.
						iff::iff_file(temp_object.parentTree()->getFile(temp_object), t);

						//Add the file to the loaded file map for safe keeping
						loadedFileMap_.insert(std::make_pair<std::string, boost::any>(filename, t));
						
						//Call the postBuild functor if we were given one
						if(postBuildProcess != nullptr)
						{
							postBuildProcess(t, *this);
						}

						//Finally return our loaded file
						return t;
					}
				}
			}

			//Ruh roh! We didn't find the file requested anywhere.
			//Oh well, not our problem anymore.
			return nullptr;
		}

		template <class T> std::vector<std::shared_ptr<T>> file_manager::load_directory(const std::string& directory, const std::string& ext, std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess)
		{
			//First create the result vector and a set to keep track of what we've already loaded
			std::vector<std::shared_ptr<T>> result;
			std::set<std::string> previously_loaded;

			//Next, we see if the base directory exists on the filesystem
			boost::filesystem2::path searchDir(basedir_ / directory);
			if(boost::filesystem::exists(searchDir))
			{
				//It does! Okay, lets iterate through it (non recursively)
				boost::filesystem::directory_iterator end_itr;
				for(boost::filesystem::directory_iterator itr(searchDir); itr != end_itr; ++itr)
				{
					if(boost::filesystem::is_regular_file(itr->status()))
					{
						//This a normal, run of the mill, file, so come up with the swg name for it
						std::string swg_name = getSWGPath_(basedir_.string(), itr->string());

						//If we were told to filter to a certain extension, we should do that now.
						if(!endsWith_(ext, swg_name))
							continue;

						//Now we quickly see if we've already loaded this swg file.
						auto lfmi = loadedFileMap_.find(swg_name);
						if(lfmi == loadedFileMap_.end())
						{
							//Well, we haven't, so lets make it real quick
							std::shared_ptr<T> t = std::make_shared<T>(); //Create the necessary interpreter
							
							//We don't actually need the iff file, so anonymously create it.
							iff::iff_file(itr->path().file_string(), t);

							//Put the interpreter in the loadedFileMap for safe keeping
							loadedFileMap_.insert(std::make_pair<std::string, boost::any>(swg_name, t));
							
							//And call the post build process if we have one
							if(postBuildProcess != nullptr)
							{
								postBuildProcess(t, *this);
							}

							//Finally put the interpreter into the vector for return to the caller
							result.push_back(t);
						}
						else
						{
							//We already loaded it! Lets put it into the vector anyway to be nice.
							result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
						}

						//Either way, we loaded the file, so lets make sure we don't try to load it again
						//or add it into the result vector.
						previously_loaded.insert(std::move(swg_name));
					}
				}
			}

			//Because we use a std::map, the entries are sorted lexicographically, so
			//we can optimize this by realizing that once we've passed our "chunk" we will never
			//find a valid file again. To do this, we have the found_chunk bool.
			bool found_chunk = false;

			//And we'll go through the WHOLE FileMap...but only in the worst case.
			FileMapIterator it = fileMap_.begin();
			FileMapIterator end = fileMap_.end();
			while(it != end)
			{
				//First we make sure that we get files that are in this directory, but not any sub directories
				//also we need to filter out any files that don't begin with the extension we want if we were given one.
				if(beginsWithNoDir_(directory, it->first) && endsWith_(ext, it->first))
				{
					//We found our chunk now, so lets set the found_chunk bool to true
					found_chunk = true;

					//Now we make sure that we didn't load it above.
					if(previously_loaded.find(it->first) == previously_loaded.end())
					{
						//But don't forget to make sure we didn't load it already in
						//an entirely different call.
						auto lfmi = loadedFileMap_.find(it->first);
						if(lfmi == loadedFileMap_.end())
						{
							//Load it

							std::shared_ptr<T> t = std::make_shared<T>(); //create the interpreter we need
							
							//Create the temp_obj to make the compiler happy
							auto temp_obj = it->second;

							//Create the iff file anonymously because we don't actually need it
							iff::iff_file(temp_obj.parentTree()->getFile(temp_obj), t);

							//We now add it to the loadedFilemap to make sure that we don't reload it
							loadedFileMap_.insert(std::make_pair<std::string, boost::any>(it->first, t));

							//And call the post build process if we had one
							if(postBuildProcess != nullptr)
							{
								postBuildProcess(t, *this);
							}

							//Finally we put it into the vector for safe keeping.
							result.push_back(t);
						}
						else
						{
							//we did! so lets put it in the vector and be done with it
							result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
						}
					}
					
				}
				else if(found_chunk)
				{
					// We've passed our chunk, so we're done!
					break; 
				}
				++it;
			}

			//Return the result!
			return result;
		}

		template <class T> std::vector<std::shared_ptr<T>> file_manager::load_all(const std::string& directory, const std::string& ext, std::function<void(std::shared_ptr<T>, file_manager&)> postBuildProcess)
		{
			//First create the result vector and a set to keep track of what we've already loaded
			std::vector<std::shared_ptr<T>> result;
			std::set<std::string> previously_loaded;

			//Next, we see if the base directory exists on the filesystem
			boost::filesystem2::path searchDir(basedir_ / directory);
			if(boost::filesystem::exists(searchDir))
			{
				//It does! Okay, lets iterate through it (recursively)
				boost::filesystem::recursive_directory_iterator end_itr;
				for(boost::filesystem::recursive_directory_iterator itr(searchDir); itr != end_itr; ++itr)
				{
					if(boost::filesystem::is_regular_file(itr->status()))
					{
						//This a normal, run of the mill, file, so come up with the swg name for it
						std::string swg_name = getSWGPath_(basedir_.string(), itr->string());

						//If we were told to filter to a certain extension, we should do that now.
						if(!endsWith_(ext, swg_name))
							continue;

						//Now we quickly see if we've already loaded this swg file.
						auto lfmi = loadedFileMap_.find(swg_name);
						if(lfmi == loadedFileMap_.end())
						{
							//Well, we haven't, so lets make it real quick
							std::shared_ptr<T> t = std::make_shared<T>(); //Create the necessary interpreter

							//We don't actually need the iff file, so anonymously create it.
							iff::iff_file(itr->path().file_string(), t);

							//Put the interpreter in the loadedFileMap for safe keeping
							loadedFileMap_.insert(std::make_pair<std::string, boost::any>(swg_name, t));

							//And call the post build process if we have one
							if(postBuildProcess != nullptr)
							{
								postBuildProcess(t, *this);
							}

							//Finally put the interpreter into the vector for return to the caller
							result.push_back(t);
						}
						else
						{
							//We already loaded it! Lets put it into the vector anyway to be nice.
							result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
						}

						//Either way, we loaded the file, so lets make sure we don't try to load it again
						//or add it into the result vector.
						previously_loaded.insert(std::move(swg_name));
					}
				}
			}

			//Because we use a std::map, the entries are sorted lexicographically, so
			//we can optimize this by realizing that once we've passed our "chunk" we will never
			//find a valid file again. To do this, we have the found_chunk bool.
			bool found_chunk = false;

			//And we'll go through the WHOLE FileMap...but only in the worst case.
			FileMapIterator it = fileMap_.begin();
			FileMapIterator end = fileMap_.end();
			while(it != end)
			{
				//First we make sure that we get files that are in this directory, but not any sub directories
				//also we need to filter out any files that don't begin with the extension we want if we were given one.
				if(beginsWith_(directory, it->first) && endsWith_(ext, it->first))
				{
					//We found our chunk now, so lets set the found_chunk bool to true
					found_chunk = true;

					//Now we make sure that we didn't load it above.
					if(previously_loaded.find(it->first) == previously_loaded.end())
					{
						//But don't forget to make sure we didn't load it already in
						//an entirely different call.
						auto lfmi = loadedFileMap_.find(it->first);
						if(lfmi == loadedFileMap_.end())
						{
							//Load it
							std::shared_ptr<T> t = std::make_shared<T>(); //create the interpreter we need

							//Create the temp_obj to make the compiler happy
							auto temp_obj = it->second;

							//Create the iff file anonymously because we don't actually need it
							iff::iff_file(temp_obj.parentTree()->getFile(temp_obj), t);

							//We now add it to the loadedFilemap to make sure that we don't reload it
							loadedFileMap_.insert(std::make_pair<std::string, boost::any>(it->first, t));

							//And call the post build process if we had one
							if(postBuildProcess != nullptr)
							{
								postBuildProcess(t, *this);
							}

							//Finally we put it into the vector for safe keeping.
							result.push_back(t);
						}
						else
						{
							//we did! so lets put it in the vector and be done with it
							result.push_back(boost::any_cast<std::shared_ptr<T>>(lfmi->second));
						}
					}
					
				}
				else if(found_chunk)
				{
					// We've passed our chunk, so we're done!
					break; 
				}
				++it;
			}

			//Return the result!
			return result;
		}
	};
};
#endif