// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef SWGANH_OBJECT_HARVESTER_INSTALLATION_HARVESTER_INSTALLATION_H_
#define SWGANH_OBJECT_HARVESTER_INSTALLATION_HARVESTER_INSTALLATION_H_

#include "swganh/object/installation/installation.h"

namespace swganh {
namespace object {
namespace harvester_installation {

class HarvesterInstallationFactory;
class HarvesterInstallationMessageBuilder;
class HarvesterInstallation : public swganh::object::installation::Installation
{
public:
	typedef HarvesterInstallationFactory FactoryType;
    typedef HarvesterInstallationMessageBuilder MessageBuilderType;

	HarvesterInstallation();

    /**
     * @return The type of this object instance.
     */
    uint32_t GetType() const;

    const static uint32_t type = 0x48494e4f;
    
private:
};

}}}  // namespace swganh::object::installation

#endif  // SWGANH_OBJECT_INSTALLATION_HARVESTER_INSTALLATION_H_
