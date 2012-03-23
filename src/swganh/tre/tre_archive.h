// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_TRE_TRE_ARCHIVE_H_
#define SWGANH_TRE_TRE_ARCHIVE_H_

#include <cstdint>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace swganh {
namespace tre {

    template<typename T>
    class TreArchive
    {
    public:
        explicit TreArchive(std::vector<std::shared_ptr<T>> readers)
            : readers_(readers)
        {        
            std::for_each(
                begin(readers_),
                end(readers_),
                [this] (const shared_ptr<T>& reader)
            {
                reader->Initialize();
            });
        }

        uint32_t GetResourceSize(const std::string& resource_name) const
        {
            auto end = readers_.end();
            for (auto iter = readers_.begin(); iter != end; ++iter)
            {
                if ((*iter)->ContainsResource(resource_name))
                {
                    return (*iter)->GetResourceSize(resource_name);
                }
            }

            throw std::runtime_error("Requested unknown resource " + resource_name);
        }

        std::vector<char> GetResource(const std::string& resource_name)
        {
            auto end = readers_.end();
            for (auto iter = readers_.begin(); iter != end; ++iter)
            {
                if ((*iter)->ContainsResource(resource_name))
                {
                    return (*iter)->GetResource(resource_name);
                }
            }
        
            throw std::runtime_error("Requested unknown resource " + resource_name);
        }
        
        std::string TreArchive::GetMd5Hash(const std::string& resource_name) const
        {
            auto end = readers_.end();
            for (auto iter = readers_.begin(); iter != end; ++iter)
            {
                if ((*iter)->ContainsResource(resource_name))
                {
                    return (*iter)->GetMd5Hash(resource_name);
                }
            }
        
            throw std::runtime_error("Requested unknown resource " + resource_name);
        }
        
        const std::vector<std::string>& TreArchive::GetTreFilenames() const
        {
            std::vector<std::string> filenames;
        
            for_each(
                begin(readers_),
                end(readers_),
                [&filenames] (const ReaderList::value_type& reader)
            {
                filenames.push_back(reader->GetFilename());
            });
        
            return filenames;
        }
        
        std::vector<std::string> TreArchive::GetAvailableResources() const
        {
            std::vector<std::string> resource_list;
        
            for_each(
                begin(tre_list_),
                end(tre_list_),
                [&resource_list] (const ReaderList::value_type& reader)
            {
                auto resources = reader->GetResourceNames();
                resource_list.insert(begin(resource_list), begin(resources), end(resources));
            });
        
            return resource_list;
        }

    private:
        typedef std::vector<std::shared_ptr<T>> ReaderList;
        ReaderList readers_;
    };
}}  // namespace swganh::tre

#endif  // SWGANH_TRE_TRE_ARCHIVE_H_
