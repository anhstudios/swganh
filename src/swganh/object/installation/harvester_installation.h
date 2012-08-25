// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE
#pragma once

#include "swganh/object/installation/installation.h"

namespace swganh {
namespace object {
namespace installation {

class HarvesterInstallation : public Installation
{
public:
    /**
     * @return The type of this object instance.
     */
    uint32_t GetType() const;

    const static uint32_t type = 0x48494e4f;
    
private:
};

}}}  // namespace swganh::object::installation
