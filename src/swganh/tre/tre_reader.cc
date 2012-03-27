// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#include "tre_reader.h"

#include <array>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

#include <zlib.h>

#include "tre_data.h"

using namespace swganh::tre;

using std::find_if;
using std::for_each;
using std::ifstream;
using std::ios_base;
using std::lock_guard;
using std::mutex;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

class TreReader::TreReaderImpl
{
public:
    TreReaderImpl(std::string filename);
    
    void Initialize();

    bool IsInitialized() const;

    bool ContainsResource(const std::string& resource_name) const;
    
    uint32_t GetResourceCount() const;

    const string& GetFilename() const;
    vector<string> GetResourceNames() const;

    vector<char> GetResource(const TreResourceInfo& resource_info);
    string GetMd5Hash(const string& resource_name) const;
    uint32_t GetResourceSize(const string& resource_name) const;

    const TreResourceInfo& GetResourceInfo(const string& resource_name) const;

private:
    TreReaderImpl();

    void ReadHeader();
    void ReadIndex();
            
    vector<TreResourceInfo> ReadResourceBlock();
    vector<char> ReadNameBlock();

    typedef std::array<char, 16> Md5Sum;
    vector<Md5Sum> ReadMd5SumBlock();
    
    void ValidateFileType(string file_type) const;
    void ValidateFileVersion(string file_version) const;

    void ReadDataBlock(
		uint32_t offset,
		uint32_t compression,
		uint32_t compressed_size, 
		uint32_t uncompressed_size, 
		char* buffer);

    bool initialized_;

    ifstream input_stream_;
    string filename_;
    TreHeader header_;

    mutex mutex_;

    vector<TreResourceInfo> resource_block_;
    vector<char> name_block_;
    vector<Md5Sum> md5sum_block_;
};

TreReader::TreReader()
: impl_(nullptr)
{}

TreReader::TreReader(const string& filename)
: impl_(new TreReaderImpl(filename))
{}

uint32_t TreReader::GetResourceCount() const
{
    return impl_->GetResourceCount();
}

vector<string> TreReader::GetResourceNames() const
{
    return impl_->GetResourceNames();
}

const string& TreReader::GetFilename() const
{
    return impl_->GetFilename();
}

vector<char> TreReader::GetResource(const string& resource_name)
{
    return impl_->GetResource(
		impl_->GetResourceInfo(resource_name));
}

bool TreReader::ContainsResource(const string& resource_name) const
{
    return impl_->ContainsResource(resource_name);
}

string TreReader::GetMd5Hash(const string& resource_name) const
{
    return impl_->GetMd5Hash(resource_name);
}

uint32_t TreReader::GetResourceSize(const string& resource_name) const
{
    return impl_->GetResourceSize(resource_name);
}

TreReader::TreReaderImpl::TreReaderImpl(std::string filename)
: filename_(filename)
{
    input_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    input_stream_.open(filename_.c_str(), ios_base::binary);

    ReadHeader();
    ReadIndex();
}

uint32_t TreReader::TreReaderImpl::GetResourceCount() const
{
    return header_.resource_count;
}

const string& TreReader::TreReaderImpl::GetFilename() const
{
    return filename_;
}

vector<string> TreReader::TreReaderImpl::GetResourceNames() const
{
    vector<string> resource_names;

    for_each(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_names] (const TreResourceInfo& info)
    {
        resource_names.push_back(&name_block_[info.name_offset]);
    });

    return resource_names;
}

vector<char> TreReader::TreReaderImpl::GetResource(const TreResourceInfo& file_info)
{
    vector<char> data(file_info.data_size); 
    
    ReadDataBlock(
        file_info.data_offset, 
        file_info.data_compression, 
        file_info.data_compressed_size, 
        file_info.data_size, 
        &data[0]);

    return data;
}

bool TreReader::TreReaderImpl::ContainsResource(const string& resource_name) const
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

string TreReader::TreReaderImpl::GetMd5Hash(const string& resource_name) const
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
        [&ss] (unsigned char c) 
    {
        ss << static_cast<unsigned>(c);
    });

    return ss.str();
}

uint32_t TreReader::TreReaderImpl::GetResourceSize(const string& resource_name) const
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
         
    return find_iter->data_size;
}

const TreResourceInfo& TreReader::TreReaderImpl::GetResourceInfo(const string& resource_name) const
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

void TreReader::TreReaderImpl::ReadHeader()
{
    {
        lock_guard<mutex> lg(mutex_);
        input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    }

    ValidateFileType(string(header_.file_type, 4));
    ValidateFileVersion(string(header_.file_version, 4));        
}

void TreReader::TreReaderImpl::ReadIndex()
{
    resource_block_ = ReadResourceBlock();
    name_block_ = ReadNameBlock();
    md5sum_block_ = ReadMd5SumBlock();
}

vector<TreResourceInfo> TreReader::TreReaderImpl::ReadResourceBlock()
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
        
vector<char> TreReader::TreReaderImpl::ReadNameBlock()
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
        
vector<TreReader::TreReaderImpl::Md5Sum> TreReader::TreReaderImpl::ReadMd5SumBlock()
{    
    uint32_t offset = header_.info_offset
        + header_.info_compressed_size
        + header_.name_compressed_size;
    uint32_t size = header_.resource_count * 16; // where 16 is the length of a md5 sum
        
    vector<Md5Sum> data(size);
    
    {
        lock_guard<mutex> lg(mutex_);
        input_stream_.seekg(offset, ios_base::beg);
        input_stream_.read(reinterpret_cast<char*>(&data[0]), size);
    }

    return data;
}

void TreReader::TreReaderImpl::ValidateFileType(string file_type) const
{
    if (file_type.compare("EERT") != 0)
    {
        throw runtime_error("Invalid tre file format");
    }
}

void TreReader::TreReaderImpl::ValidateFileVersion(string file_version) const
{
    if (file_version.compare("5000") != 0)
    {
        throw runtime_error("Invalid tre file version");
    }
}

void TreReader::TreReaderImpl::ReadDataBlock(
    uint32_t offset,
    uint32_t compression,
    uint32_t compressed_size, 
    uint32_t uncompressed_size, 
    char* buffer)
{    
    if (compression == 0)
    {
        {
            lock_guard<mutex> lg(mutex_);
            input_stream_.seekg(offset, ios_base::beg);
            input_stream_.read(buffer, uncompressed_size);
        }
    }
    else if (compression == 2)
    {
        vector<char> compressed_data(compressed_size);
        
        {
            lock_guard<mutex> lg(mutex_);
            input_stream_.seekg(offset, ios_base::beg);
            input_stream_.read(&compressed_data[0], compressed_size);
        }

        int result = uncompress(
            reinterpret_cast<Bytef*>(buffer),
            reinterpret_cast<uLongf*>(&uncompressed_size),
            reinterpret_cast<Bytef*>(&compressed_data[0]),
            compressed_size);

        if (result != Z_OK)
        {
            throw std::runtime_error("ZLib error: " + result);
        }
    }
    else
    {
        throw std::runtime_error("Unknown format");
    }
}
