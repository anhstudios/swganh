// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "tre_archive.h"

#include "config_reader.h"
    
using namespace swganh::tre;

using std::for_each;
using std::move;
using std::runtime_error;
using std::string;
using std::vector;

TreArchive::TreArchive(vector<TreReader>&& readers)
    : readers_(move(readers))
{}

TreArchive::TreArchive(vector<string>&& resource_files)
{
    CreateReaders(resource_files);
}

TreArchive::TreArchive(string config_file)
{
    ConfigReader config_reader(config_file);

    CreateReaders(config_reader.GetTreFilenames());
}

uint32_t TreArchive::GetResourceSize(const string& resource_name) const
{
    for (auto& reader : readers_)
    {
        if (reader.ContainsResource(resource_name))
        {
            return reader.GetResourceSize(resource_name);
        }
    }

    throw runtime_error("Requested unknown resource " + resource_name);
}

vector<char> TreArchive::GetResource(const string& resource_name)
{
    for (auto& reader : readers_)
    {
        if (reader.ContainsResource(resource_name))
        {
            return reader.GetResource(resource_name);
        }
    }

    throw runtime_error("Requested unknown resource " + resource_name);
}

string TreArchive::GetMd5Hash(const string& resource_name) const
{
    for (auto& reader : readers_)
    {
        if (reader.ContainsResource(resource_name))
        {
            return reader.GetMd5Hash(resource_name);
        }
    }

    throw runtime_error("Requested unknown resource " + resource_name);
}

vector<string> TreArchive::GetTreFilenames() const
{
    vector<string> filenames;
    
    for (auto& reader : readers_)
    {
        filenames.push_back(reader.GetFilename());
    }

    return filenames;
}

vector<string> TreArchive::GetAvailableResources() const
{
    vector<string> resource_list;

    for (auto& reader : readers_)
    {
        auto resources = reader.GetResourceNames();
        resource_list.insert(begin(resource_list), begin(resources), end(resources));
    }

    return resource_list;
}


 void TreArchive::CreateReaders(const vector<string>& resource_files)
 { 
    for (auto& filename : resource_files)
    {
        readers_.emplace_back(filename);
    }
 }
