// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "tre_reader.h"
#include "swganh/byte_buffer.h"

#include <array>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <boost/thread/mutex.hpp>

#include <zlib.h>

#include "tre_data.h"

using namespace swganh::tre;

using std::find_if;
using std::for_each;
using std::ifstream;
using std::ios_base;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

TreReader::TreReader(const string& filename)
: filename_(filename)
{
    input_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    input_stream_.open(filename_.c_str(), ios_base::binary);

    ReadHeader();
    ReadIndex();
}

TreReader::~TreReader()
{
    {
        boost::lock_guard<boost::mutex> lg(mutex_);
        input_stream_.close();
    }
}

uint32_t TreReader::GetResourceCount() const
{
    return header_.resource_count;
}

const string& TreReader::GetFilename() const
{
    return filename_;
}

vector<string> TreReader::GetResourceNames() const
{
    vector<string> resource_names;

    for_each(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_names] (const TreResourceInfo& info)
    {
        resource_names.push_back(reinterpret_cast<const char*>(&name_block_[info.name_offset]));
    });

    return resource_names;
}

swganh::ByteBuffer TreReader::GetResource(const std::string& resource_name)
{
    swganh::ByteBuffer data; 
    
    GetResource(resource_name, data);

    return data;
}

void TreReader::GetResource(const std::string& resource_name, swganh::ByteBuffer& buffer)
{    
    auto file_info = GetResourceInfo(resource_name);

    if (file_info.data_size > buffer.size())
    {
        buffer.resize(file_info.data_size);
    }

    if (file_info.data_size != 0)
    {
        ReadDataBlock(
            file_info.data_offset,
            file_info.data_compression,
            file_info.data_compressed_size,
            file_info.data_size,
            (char*)(&buffer.raw()[0]));
    }

}

bool TreReader::ContainsResource(const string& resource_name) const
{
    auto find_iter = find_if(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(&name_block_[info.name_offset]) == 0;
    });

    return find_iter != end(resource_block_);
}

string TreReader::GetMd5Hash(const string& resource_name) const
{
    auto find_iter = find_if(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(&name_block_[info.name_offset]) == 0;
    });

    if (find_iter == resource_block_.end())
    {
        throw std::runtime_error("File name invalid");
    }

    stringstream ss;

    ss.flags(ss.hex);
    ss.fill('0');
    ss.width(2);
    
    for_each(
        begin(md5sum_block_[find_iter - begin(resource_block_)]), 
        begin(md5sum_block_[find_iter - begin(resource_block_)]) + sizeof(Md5Sum),
        [&ss] (char c) 
    {
        ss << static_cast<unsigned>(c);
    });

    return ss.str();
}

uint32_t TreReader::GetResourceSize(const string& resource_name) const
{
    auto find_iter = find_if(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(reinterpret_cast<const char*>(&name_block_[info.name_offset])) == 0;
    });

    if (find_iter == resource_block_.end())
    {
        throw std::runtime_error("File name invalid");
    }
         
    return find_iter->data_size;
}

const TreResourceInfo& TreReader::GetResourceInfo(const string& resource_name) const
{
    auto find_iter = find_if(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(&name_block_[info.name_offset]) == 0;
    });
    
    if (find_iter == end(resource_block_))
    {
        throw std::runtime_error("Requested info for invalid file: " + resource_name);
    }

    return *find_iter;
}

void TreReader::ReadHeader()
{
    {
        boost::lock_guard<boost::mutex> lg(mutex_);
        input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    }

    ValidateFileType(string(header_.file_type, 4));
    ValidateFileVersion(string(header_.file_version, 4));        
}

void TreReader::ReadIndex()
{
    resource_block_ = ReadResourceBlock();
    name_block_ = ReadNameBlock();
    md5sum_block_ = ReadMd5SumBlock();
}

vector<TreResourceInfo> TreReader::ReadResourceBlock()
{
    uint32_t uncompressed_size = header_.resource_count * sizeof(TreResourceInfo);
    
    vector<TreResourceInfo> files(header_.resource_count);
        
    ReadDataBlock(header_.info_offset,
        header_.info_compression,
        header_.info_compressed_size,
        uncompressed_size,
        reinterpret_cast<char*>(&files[0]));

    return files;
}
        
vector<char> TreReader::ReadNameBlock()
{
    vector<char> data(header_.name_uncompressed_size); 
    
    uint32_t name_offset = header_.info_offset + header_.info_compressed_size;

    ReadDataBlock(
        name_offset, 
        header_.name_compression, 
        header_.name_compressed_size, 
        header_.name_uncompressed_size, 
        &data[0]);

    return data;
}
        
vector<TreReader::Md5Sum> TreReader::ReadMd5SumBlock()
{    
    uint32_t offset = header_.info_offset
        + header_.info_compressed_size
        + header_.name_compressed_size;
    uint32_t size = header_.resource_count * 16; // where 16 is the length of a md5 sum
        
    vector<Md5Sum> data(size);
    
    {
        boost::lock_guard<boost::mutex> lg(mutex_);
        input_stream_.seekg(offset, ios_base::beg);
        input_stream_.read(reinterpret_cast<char*>(&data[0]), size);
    }

    return data;
}

void TreReader::ValidateFileType(string file_type) const
{
    if (file_type.compare("EERT") != 0)
    {
        throw runtime_error("Invalid tre file format");
    }
}

void TreReader::ValidateFileVersion(string file_version) const
{
    if (file_version.compare("5000") != 0)
    {
        throw runtime_error("Invalid tre file version");
    }
}

void TreReader::ReadDataBlock(
    uint32_t offset,
    uint32_t compression,
    uint32_t compressed_size, 
    uint32_t uncompressed_size, 
    char* buffer)
{    
    if (compression == 0)
    {
        {
            boost::lock_guard<boost::mutex> lg(mutex_);
            input_stream_.seekg(offset, ios_base::beg);
            input_stream_.read(buffer, uncompressed_size);
        }
    }
    else if (compression == 2)
    {
        vector<char> compressed_data(compressed_size);
        
        {
            boost::lock_guard<boost::mutex> lg(mutex_);
            input_stream_.seekg(offset, ios_base::beg);
            input_stream_.read(&compressed_data[0], compressed_size);
        }

        int result;
        z_stream stream;
        stream.zalloc = Z_NULL;
        stream.zfree = Z_NULL;
        stream.opaque = Z_NULL;
        stream.avail_in = Z_NULL;
        stream.next_in = Z_NULL;
        result = inflateInit(&stream);

        if (result != Z_OK)
        {
            throw std::runtime_error("Zlib error: " + std::to_string(result));
        }

        stream.next_in = reinterpret_cast<Bytef*>(&compressed_data[0]);
        stream.avail_in = compressed_size;
        stream.next_out = reinterpret_cast<Bytef*>(buffer);
        stream.avail_out = uncompressed_size;

        inflate(&stream, Z_FINISH);

        if(stream.total_out > 0)
        {
            inflateEnd(&stream);
        }
    }
    else
    {
        throw std::runtime_error("Unknown format");
    }
}
