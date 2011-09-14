
#ifndef SWGANH_OBJECT_TANGIBLE_HARVESTER_INSTALLATION_H_
#define SWGANH_OBJECT_TANGIBLE_HARVESTER_INSTALLATION_H_

#include "swganh/object/tangible/installation.h"

namespace swganh {
namespace object {
namespace tangible {

class HarvesterInstallation : public Installation
{
public:
    virtual uint32_t GetType() { return HarvesterInstallation::type; }
    const static uint32_t type = 0x48494e4f;
    
private:
    virtual void OnReliableUpdate();
};

}}}  // namespace swganh::object::tangible

#endif  // SWGANH_OBJECT_TANGIBLE_HARVESTER_INSTALLATION_H_
