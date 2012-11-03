// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace swganh {
namespace chat {

    struct PersistentMessage
    {
        uint32_t id;
        uint32_t timestamp;
        bool read;
        std::string game_tag;
        std::string galaxy_tag;
        std::string sender;
        std::string recipient;
        std::string subject;
        std::string message;
        std::vector<char> attachments;
    };

}}  // namespace swganh::chat
