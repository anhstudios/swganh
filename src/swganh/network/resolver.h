// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <string>

namespace swganh {
namespace network {

//// Resolves a hostname to it's ip address in string format, ex localhost to "127.0.0.1"
std::string resolve_to_string(const std::string& hostname);

}}  // namespace swganh::network
