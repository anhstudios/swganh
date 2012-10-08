// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>
#include <array>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include <boost/thread/mutex.hpp>

#include "tre_data.h"

namespace swganh {
	class ByteBuffer;
}

namespace swganh {
namespace tre {

    /**
     * TreReader is a utility class used for reading data from a single 
     * .tre file in pre-publish 15 format.
     */
    class TreReader
    {
    public:
        /**
         * Explicit constructor taking the filename of the archive. This can
         * be an explicit path or relative to the current working directory.
         *
         * \param filename The filename of the archive file to be loaded.
         */
        explicit TreReader(const std::string& filename);
        ~TreReader();

        /**
         * Checks whether a specified resource is contained within the archive.
         *
         * \param resource_name The name of the resource.
         * \return True if the resource is in the archive, false if not.
         */
        bool ContainsResource(const std::string& resource_name) const;
        
        /**
         * Returns a count of available resources in the archive.
         *
         * \return The number of resources in the archive.
         */
        uint32_t GetResourceCount() const;

        /**
         * Returns a list of the of all available resources in the archive.
         *
         * \return A list of available resources.
         */
        std::vector<std::string> GetResourceNames() const;

        /**
         * Returns the base filename of this .tre archive.
         *
         * \return The name of this archive (e.g., patch_00.tre)
         */
        const std::string& GetFilename() const;

        /**
         * Returns the size of the requested resource.
         *
         * \param resource_name The name of the resource.
         * \return The size of the requested resource.
         */
        uint32_t GetResourceSize(const std::string& resource_name) const;

        const TreResourceInfo& GetResourceInfo(const std::string& resource_name) const;

        /**
         * Returns the requested resource in binary format.
         *
         * \param resource_name The name of the resource.
         * \return The file in binary format (move constructable).
         */
        swganh::ByteBuffer GetResource(const std::string& resource_name);

        void GetResource(const std::string& resource_name, swganh::ByteBuffer& buffer);

        /**
         * Returns the md5 hash of the requested resource.
         *
         * \param resource_name The name of the resource.
         * \return The md5 hash of the requseted resource.
         */
        std::string GetMd5Hash(const std::string& resource_name) const;
    
    private:
        TreReader();

        void ReadHeader();
        void ReadIndex();
                
        std::vector<TreResourceInfo> ReadResourceBlock();
        std::vector<char> ReadNameBlock();

        typedef std::array<char, 16> Md5Sum;
        std::vector<Md5Sum> ReadMd5SumBlock();
        
        void ValidateFileType(std::string file_type) const;
        void ValidateFileVersion(std::string file_version) const;

        void ReadDataBlock(
	    	uint32_t offset,
	    	uint32_t compression,
	    	uint32_t compressed_size, 
	    	uint32_t uncompressed_size, 
	    	char* buffer);

        bool initialized_;

        typedef std::ifstream BinaryStream;
        BinaryStream input_stream_;
        std::string filename_;
        TreHeader header_;

        boost::mutex mutex_;

        std::vector<TreResourceInfo> resource_block_;
        std::vector<char> name_block_;
        std::vector<Md5Sum> md5sum_block_;
    };

}}  // namespace swganh::tre
