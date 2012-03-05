
#ifndef SWGANH_OBJECT_INSTALLATION_HARVESTER_INSTALLATION_H_
#define SWGANH_OBJECT_INSTALLATION_HARVESTER_INSTALLATION_H_

#include "swganh/object/installation/installation.h"

namespace swganh {
namespace object {
namespace installation {

class HarvesterInstallation : public Installation
{
public:
    /**
     * \return The type of this object instance.
     */
    uint32_t GetType() const;

    const static uint32_t type = 0x48494e4f;
    
private:
};

}}}  // namespace swganh::object::installation

#endif  // SWGANH_OBJECT_INSTALLATION_HARVESTER_INSTALLATION_H_
