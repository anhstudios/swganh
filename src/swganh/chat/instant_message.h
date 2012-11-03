// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#pragma once

#include <string>

namespace swganh {
namespace chat {

    struct InstantMessage
    {
        std::string game_tag;
        std::string galaxy_tag;
        std::string sender;
        std::string recipient;
        std::string message;
    };

}}  // namespace swganh::chat
