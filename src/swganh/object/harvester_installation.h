
#ifndef SWGANH_OBJECT_HARVESTER_INSTALLATION_H_
#define SWGANH_OBJECT_HARVESTER_INSTALLATION_H_

#include "swganh/object/installation.h"

namespace swganh {
namespace object {

class HarvesterInstallation : public Installation
{
public:
    virtual uint32_t GetType() { return HarvesterInstallation::type; }
    const static uint32_t type = 0x48494e4f;
    
private:
    virtual void OnReliableUpdate();
};

}}  // namespace swganh::object

#endif  // SWGANH_OBJECT_HARVESTER_INSTALLATION_H_
