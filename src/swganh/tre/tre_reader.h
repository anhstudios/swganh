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

    class TreReader
    {
    public:
        explicit TreReader(const std::string& filename);

        void Initialize();
        
        bool ContainsResource(const std::string& resource_name) const;
        
        uint32_t GetResourceCount() const;

        std::vector<std::string> GetResourceNames() const;

        const std::string& GetFilename() const;

        uint32_t GetResourceSize(const std::string& resource_name) const;

        std::vector<char> GetResource(const std::string& resource_name);

        std::string GetMd5Hash(const std::string& resource_name) const;
    
    private:
        TreReader();

        void VerifyInitialization() const;

        class TreReaderImpl;
        std::shared_ptr<TreReaderImpl> impl_;
    };

}}  // namespace swganh::tre

#endif  // SWGANH_TRE_TRE_READER_H_
