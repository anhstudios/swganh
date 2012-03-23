// This file is part of SWGANH which is released under GPL v2.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_TRE_LIVE_READER_H_
#define SWGANH_TRE_LIVE_READER_H_

#include <string>
#include <vector>

namespace swganh {
namespace tre {

    class ConfigReader
    {
    public:
        explicit ConfigReader(std::string filename);

        const std::vector<std::string>& GetTreFilenames();

    public:
        void ParseConfig();

        std::vector<std::string> tre_filenames_;
        std::string config_filename_;
    };

}}  // namespace swganh::tre

#endif  // SWGANH_TRE_LIVE_READER_H_
