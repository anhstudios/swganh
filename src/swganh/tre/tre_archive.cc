// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "tre_archive.h"

#include "config_reader.h"
#include "swganh/byte_buffer.h"
    
using namespace swganh::tre;

using std::for_each;
using std::move;
using std::runtime_error;
using std::string;
using std::vector;

TreArchive::TreArchive(vector<std::unique_ptr<TreReader>>&& readers)
    : readers_(move(readers))
{}

TreArchive::TreArchive(vector<string>&& resource_files)
{
    CreateReaders(resource_files);
}

TreArchive::~TreArchive()
{
    readers_.clear();
}

TreArchive::TreArchive(string config_file)
{
    ConfigReader config_reader(config_file);

    CreateReaders(config_reader.GetTreFilenames());
}

bool TreArchive::Open()
{
    return true;
}

uint32_t TreArchive::GetResourceSize(const string& resource_name) const
{
    for (auto& reader : readers_)
    {
        if (reader->ContainsResource(resource_name))
        {
            return reader->GetResourceSize(resource_name);
        }
    }

    throw runtime_error("Requested unknown resource " + resource_name);
}

swganh::ByteBuffer TreArchive::GetResource(const string& resource_name)
{
    for (auto& reader : readers_)
    {
        if (reader->ContainsResource(resource_name))
        {
            return reader->GetResource(resource_name);
        }
    }

    throw runtime_error("Requested unknown resource " + resource_name);
}

void TreArchive::GetResource(const std::string& resource_name, swganh::ByteBuffer& buffer)
{
    for (auto& reader : readers_)
    {
        if (reader->ContainsResource(resource_name))
        {
            return reader->GetResource(resource_name, buffer);
        }
    }

    throw runtime_error("Requested unknown resource " + resource_name);
}

string TreArchive::GetMd5Hash(const string& resource_name) const
{
    for (auto& reader : readers_)
    {
        if (reader->ContainsResource(resource_name))
        {
            return reader->GetMd5Hash(resource_name);
        }
    }

    throw runtime_error("Requested unknown resource " + resource_name);
}

vector<string> TreArchive::GetTreFilenames() const
{
    vector<string> filenames;
    
    for (auto& reader : readers_)
    {
        filenames.push_back(reader->GetFilename());
    }

    return filenames;
}


vector<string> TreArchive::GetAvailableResources() const
{
    return GetAvailableResources(std::function<void (int total, int completed)>());
}

std::vector<std::string> TreArchive::GetAvailableResources(std::function<void (int total, int completed)> progress_callback) const
{
    vector<string> resource_list;

    int total = readers_.size();
    int completed = 0;
    for (auto& reader : readers_)
    {
        auto resources = reader->GetResourceNames();
        resource_list.insert(begin(resource_list), begin(resources), end(resources));

        ++completed;
        progress_callback(total, completed);
    }

    // sort and remove duplicates
    std::sort(std::begin(resource_list), std::end(resource_list));
    auto erase_iter = std::unique(resource_list.begin(), resource_list.end());
    resource_list.erase(erase_iter, resource_list.end());

    return resource_list;
}


 void TreArchive::CreateReaders(const vector<string>& resource_files)
 { 
    for (auto& filename : resource_files)
    {
        readers_.push_back(std::unique_ptr<TreReader>(new TreReader(filename)));
    }
 }
