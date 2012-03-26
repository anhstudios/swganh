// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_TRE_TRE_READER_H_
#define SWGANH_TRE_TRE_READER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

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

        /**
         * Returns the requested resource in binary format.
         *
         * \param resource_name The name of the resource.
         * \return The file in binary format (move constructable).
         */
        std::vector<char> GetResource(const std::string& resource_name);

        /**
         * Returns the md5 hash of the requested resource.
         *
         * \param resource_name The name of the resource.
         * \return The md5 hash of the requseted resource.
         */
        std::string GetMd5Hash(const std::string& resource_name) const;
    
    private:
        TreReader();

        class TreReaderImpl;
        std::shared_ptr<TreReaderImpl> impl_;
    };

}}  // namespace swganh::tre

#endif  // SWGANH_TRE_TRE_READER_H_
