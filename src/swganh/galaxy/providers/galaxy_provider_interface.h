// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include <cstdint>

namespace swganh {
namespace galaxy {
namespace providers {

class GalaxyProviderInterface {
public:
    virtual ~GalaxyProviderInterface() {}

    virtual uint32_t GetPopulation() = 0;
};

}}}  // namespace swganh::galaxy::providers
